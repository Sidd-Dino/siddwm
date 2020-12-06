// siddwm - siddwm is dumb dumb wm
// refer [https://tronche.com/gui/x/xlib/] for basic explanations

#include <X11/Xlib.h>
#include <X11/XF86keysym.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "siddwm.h"

/* Globals
 list    : list of windows of current workspace
           (dynamic array of clients)
 ws_list : list of workspaces (array of pointers) (array of clients)
 cur     : current window in focus
 ws      : current workspace
 sw , sh : screen width and height
 wx , wy : top left corner of the current window
 numlock : Numlock mask used in input_grab()
 ww , wh : window width and height

 d       : connection to X-server
 mouse   : Read this link
           [https://tronche.com/gui/x/xlib/events/keyboard-pointer/
            keyboard-pointer.html#XButtonEvent]
 root    : root screen
*/
static client       *list = {0}, *ws_list[10] = {0}, *cur;
static int          ws = 1, sw, sh, numlock = 0;

static Display      *d;
static XButtonEvent mouse;
static Window       root;

static int end_loop = 0;

/* General Info
  XEvent union info : https://tronche.com/gui/x/xlib/events/structures.html
*/

// Array of fucntion pointer of Xevents to custom fucntions
static void (*events[LASTEvent])(XEvent *e) = {
    [ButtonPress]      = button_press,
    [ButtonRelease]    = button_release,
    [ConfigureRequest] = configure_request,
    [KeyPress]         = key_press,
    [MapRequest]       = map_request,
    [MappingNotify]    = mapping_notify,
    [DestroyNotify]    = notify_destroy,
    [EnterNotify]      = notify_enter,
    [MotionNotify]     = notify_motion,
};

#include "config.h"

/* on the press of a button check to see if there's a binded
 *function to call
 */
void button_press(XEvent *e) {
    if (!e->xbutton.subwindow) return;

    win_size(e->xbutton.subwindow, &cur->wx, &cur->wy, &cur->ww, &cur->wh);
    XRaiseWindow(d, e->xbutton.subwindow);
    mouse = e->xbutton;
}

/* on the release of a button check to see if there's a binded
 * function to call
 */
void button_release(XEvent *e) {
    mouse.subwindow = 0;
}

/* a configure request means that the window requested changes in its geometry
 * state.
 */
void configure_request(XEvent *e) {
    XConfigureRequestEvent *ev = &e->xconfigurerequest;

    XConfigureWindow(d, ev->window, ev->value_mask, &(XWindowChanges) {
        .x          = ev->x,
        .y          = ev->y,
        .width      = ev->width,
        .height     = ev->height,
        .sibling    = ev->above,
        .stack_mode = ev->detail
    });
}

/* on the press of a key check to see if there's a binded function to call */
void key_press(XEvent *e) {
    KeySym keysym = XkbKeycodeToKeysym(d, e->xkey.keycode, 0, 0);

    for (unsigned int i=0; i < sizeof(keys)/sizeof(*keys); ++i)
        if (keys[i].keysym == keysym &&
            mod_clean(keys[i].mod) == mod_clean(e->xkey.state))
            keys[i].function(keys[i].arg);
}

/* a map request is received when a window wants to display itself */
void map_request(XEvent *e) {
    Window w = e->xmaprequest.window;

    XSelectInput(d, w, StructureNotifyMask|EnterWindowMask);
    win_add(w);
    cur = list->prev;
    win_size(w, &cur->wx, &cur->wy, &cur->ww, &cur->wh);

    if (cur->wx + cur->wy == 0) win_center((Arg){0});

    XMapWindow(d, w);
    win_focus(list->prev);
}

/* ??? WTF is this for
 * I guess this explains it?
 * https://tronche.com/gui/x/xlib/events/window-state-change/mapping.html
 */
void mapping_notify(XEvent *e) {
    XMappingEvent *ev = &e->xmapping;

    if (ev->request == MappingKeyboard || ev->request == MappingModifier) {
        XRefreshKeyboardMapping(ev);
        input_grab(root);
    }
}

/* a destroy notification is received when a window is being closed
 * on receival, remove the appropriate client that held that window
 */
void notify_destroy(XEvent *e) {
    int tmp = ws;

    for (int i = 0; i<10; i++) {
        ws_sel(i);
        win_del(e->xdestroywindow.window , 1 );
    }

    ws_sel(tmp);

    if (list) win_focus(list->prev);
}

/* when the mouse hovers over a window focus on that window */
void notify_enter(XEvent *e) {
    // XCheckTypedEvent searches for EnterNotify event in the event queue and
    // returns that event as e
    // rtfm (https://tronche.com/gui/x/xlib/events/window-entry-exit/)
    while(XCheckTypedEvent(d, EnterNotify, e));

    for win if (c->w == e->xcrossing.window) win_focus(c);
}

/* if the mouse moves while over a window, execute code */
void notify_motion(XEvent *e) {
    if (!mouse.subwindow || cur->f) return;

    while(XCheckTypedEvent(d, MotionNotify, e));

    int xd = e->xbutton.x_root - mouse.x_root;
    int yd = e->xbutton.y_root - mouse.y_root;

    XMoveResizeWindow(d, mouse.subwindow,
        cur->wx + (mouse.button == 1 ? xd : 0),
        cur->wy + (mouse.button == 1 ? yd : 0),
        MAX(1, cur->ww + (mouse.button == 3 ? xd : 0)),
        MAX(1, cur->wh + (mouse.button == 3 ? yd : 0)));
}


/* The data structure used by sowm and siddwm is a
 * circular linked list.
 */
void win_add(Window w) {
    client *c;

    if (!(c = (client *) calloc(1, sizeof(client))))
        exit(1);

    c->w = w;

    if (list) {
        list->prev->next = c;
        c->prev          = list->prev;
        list->prev       = c;
        c->next          = list;
    } else {
        list = c;
        list->prev = list->next = list;
    }

    ws_save(ws);
}

void win_center(const Arg arg) {
    if (!cur) return;

    win_size(cur->w, &(int){0}, &(int){0}, &cur->ww, &cur->wh);
    XMoveWindow(d, cur->w, (sw - cur->ww) / 2, (sh - cur->wh) / 2);
}

void win_close(const Arg arg) {
    XEvent ev;
    ev.type = ClientMessage;
    ev.xclient.window = cur->w;
    ev.xclient.message_type = XInternAtom(d, "WM_PROTOCOLS", 1);
    ev.xclient.format = 32;
    ev.xclient.data.l[0] = XInternAtom(d, "WM_DELETE_WINDOW", 0);
    ev.xclient.data.l[1] = CurrentTime;
    XSendEvent(d, cur->w, False, NoEventMask, &ev);
}

void win_del(Window w, int del_cur) {
    // del_cur is used if win_del is called by notify_destroy
    // to empty cur else it shouldnt empty cur
    client *x = 0;

    for win if (c->w == w) x = c;

    if (!list || !x)  return;
    if (x->prev == x){
        list = 0;
        cur = del_cur ? 0 : cur;
    }
    if (list == x)    list = x->next;
    if (x->next)      x->next->prev = x->prev;
    if (x->prev)      x->prev->next = x->next;

    free(x);
    ws_save(ws);
}

void win_focus(client *c) {
    cur = c;
    XSetInputFocus(d, cur->w, RevertToParent, CurrentTime);
}

void win_fs(const Arg arg) {
    if (!cur) return;

    if ((cur->f = cur->f ? 0 : 1)) {
        win_size(cur->w, &cur->wx, &cur->wy, &cur->ww, &cur->wh);
        XMoveResizeWindow(d, cur->w, 0, 0, sw, sh);

    } else {
        XMoveResizeWindow(d, cur->w, cur->wx, cur->wy, cur->ww, cur->wh);
    }
}

void win_kill(const Arg arg) {
    if (cur) XKillClient(d, cur->w);
}

void win_next(const Arg arg) {
    if (!cur) return;

    XRaiseWindow(d, cur->next->w);
    win_focus(cur->next);
}

void win_prev(const Arg arg) {
        if (!cur) return;

        XRaiseWindow(d, cur->prev->w);
        win_focus(cur->prev);
}

void win_resize_h(const Arg arg) {
    if (!cur) return;

    cur->wh=MAX(cur->wh+arg.i , MIN_WINDOW_SIZE );

    XResizeWindow(d, cur->w, cur->ww, cur->wh );
}

void win_resize_w(const Arg arg) {
    if (!cur) return;

    cur->ww=MAX(cur->ww+arg.i , MIN_WINDOW_SIZE);

    XResizeWindow(d, cur->w, cur->ww, cur->wh );

}


void ws_go(const Arg arg) {
    int tmp = ws;

    if (arg.i == ws) return;

    ws_save(ws);
    ws_sel(arg.i);

    for win XMapWindow(d, c->w);

    ws_sel(tmp);

    for win XUnmapWindow(d, c->w);

    ws_sel(arg.i);

    if (list) win_focus(list); else cur = 0;
}

void win_to_ws(const Arg arg) {
    if(!cur) return;

    int tmp = ws;

    if (arg.i == tmp) return;

    ws_sel(arg.i);
    win_add(cur->w);
    ws_save(arg.i);

    ws_sel(tmp);
    win_del(cur->w , 0);
    XUnmapWindow(d, cur->w);
    ws_save(tmp);

    if (list) win_focus(list);
}


void run(const Arg arg) {
    if (fork()) return;

    // if connection to X is ??? , end connection
    if (d) close(ConnectionNumber(d));

    setsid();
    execvp((char*)arg.com[0], (char**)arg.com);
}

void input_grab(Window root) {
    unsigned int i, j, modifiers[] = {0, LockMask, numlock, numlock|LockMask};
    XModifierKeymap *modmap = XGetModifierMapping(d);
    KeyCode code;

    for (i = 0; i < 8; i++)
        for (int k = 0; k < modmap->max_keypermod; k++)
            if (modmap->modifiermap[i * modmap->max_keypermod + k]
                == XKeysymToKeycode(d, 0xff7f))
                numlock = (1 << i);

    XUngrabKey(d, AnyKey, AnyModifier, root);

    for (i = 0; i < sizeof(keys)/sizeof(*keys); i++)
        if ((code = XKeysymToKeycode(d, keys[i].keysym)))
            for (j = 0; j < sizeof(modifiers)/sizeof(*modifiers); j++)
                XGrabKey(d, code, keys[i].mod | modifiers[j], root,
                        True, GrabModeAsync, GrabModeAsync);

    for (i = 1; i < 4; i += 2)
        for (j = 0; j < sizeof(modifiers)/sizeof(*modifiers); j++)
            XGrabButton(d, i, MOD | modifiers[j], root, True,
                ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
                GrabModeAsync, GrabModeAsync, 0, 0);

    XFreeModifiermap(modmap);
}


void quit_siddwm(const Arg arg) {
    for (int i = 0; i < 10; i++) {
        ws_sel(ws);
        while (list != NULL)
            win_del( list->w , 1 );
    }

    XCloseDisplay(d);
    end_loop = 1;
}

int main(void) {
    XEvent ev;

    // open new conenction with Xserver
    if (!(d = XOpenDisplay(0))) exit(1);

    // Ignore SIGCHILD
    signal(SIGCHLD, SIG_IGN);

    /*
        execute xerror on encountering error
        Read link for more info
        [ https://linux.die.net/man/3/xseterrorhandler ]
    */
    XSetErrorHandler(xerror);

    // Default screen ges the current screen
    int s = DefaultScreen(d);
    root  = RootWindow(d, s);
    sw    = XDisplayWidth(d, s);
    sh    = XDisplayHeight(d, s);

    // rtfm
    // [https://linux.die.net/man/3/xselectinput]
    XSelectInput(d,  root, SubstructureRedirectMask);

    // rtfm
    // [https://www.x.org/releases/X11R7.5/doc/man/man3/XDefineCursor.3.html]
    XDefineCursor(d, root, XCreateFontCursor(d, 68));
    input_grab(root);

    // main loop
    while (!end_loop && !XNextEvent(d, &ev)) // 1 && will forever be here.
        if (events[ev.type]) events[ev.type](&ev);
}
