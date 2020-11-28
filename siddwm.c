#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <X11/XF86keysym.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "siddwm.h"
#include "config.def.h"

static client *list = {0}, *ws_list[WORKSPACES], *cur;
static xcb_connection_t *dpy;
static xcb_screen_t *scr;
unsigned int numlockmask = 0;

static void (*events[XCB_NO_OPERATION])(xcb_generic_event_t *e)  = {
        [XCB_BUTTON_PRESS]        = buttonpress,
        [XCB_BUTTON_RELEASE]      = buttonrelease,
        [XCB_CONFIGURE_REQUEST]   = configurerequest,
        [XCB_KEY_PRESS]           = keypress,
        [XCB_MAP_REQUEST]         = maprequest,
        [XCB_MAPPING_NOTIFY]      = mapnotify,
        [XCB_DESTROY_NOTIFY]      = destroynotify,
        [XCB_ENTER_NOTIFY]        = enternotify,
        [XCB_MOTION_NOTIFY]       = motionnotify,
};

/* Yoinked from c00kiemon5ter/monsterwm */
/* wrapper to get xcb keycodes from keysymbol */
xcb_keycode_t* get_keycodes(xcb_keysym_t keysym) {
    xcb_key_symbols_t *keysyms;
    xcb_keycode_t     *keycode;

    if (!(keysyms = xcb_key_symbols_alloc(dpy))) return NULL;
    keycode = xcb_key_symbols_get_keycode(keysyms, keysym);
    xcb_key_symbols_free(keysyms);

    return keycode;
}

int grab_input(void){
    xcb_keycode_t *keycode;
    unsigned int modifiers[] = {
		0,
		XCB_MOD_MASK_LOCK,
		numlockmask,
		numlockmask | XCB_MOD_MASK_LOCK
	};

    xcb_ungrab_key(dpy, XCB_GRAB_ANY, scr->root, XCB_MOD_MASK_ANY);
    xcb_ungrab_button(dpy, XCB_BUTTON_INDEX_ANY, scr->root, XCB_GRAB_ANY);

    for (int i=0; i<LENGTH(keys); i++){
        keycode=get_keycodes(keys[i].keysym)
        for (unsigned int k=0; keycode[k] != XCB_NO_SYMBOL; k++)
            for (int m = 0; m < LENGTH(modifiers); m++)
                xcb_grab_key(dpy, 1, scr->root,
                    keys[i].mod | modifiers[m],
                    keycode[k],
                    XCB_GRAB_MODE_ASYNC,
                    XCB_GRAB_MODE_ASYNC);
    }


    for (int i=1; i<4; i++)
        for (unsigned int m=0; m<LENGTH(modifiers); m++)
            xcb_grab_button(dpy, 1, scr->root,
                XCB_EVENT_MASK_BUTTON_PRESS |
                XCB_EVENT_MASK_BUTTON_RELEASE|
                XCB_EVENT_MASK_POINTER_MOTION,
                XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC,
                scr->root, XCB_NONE, i, MOD|modifiers[m]);

    xcb_flush(dpy);
}

/* get numlock modifier using xcb */
int setup_keyboard(void){

    unsigned int modifiers[] = {
        0,
        XCB_MOD_MASK_LOCK,
        numlockmask,
        numlockmask | XCB_MOD_MASK_LOCK
    };

    xcb_get_modifier_mapping_reply_t *reply;
    xcb_keycode_t                    *modmap;
    xcb_keycode_t                    *numlock;

    reply = xcb_get_modifier_mapping_reply(dpy,
            xcb_get_modifier_mapping_unchecked(dpy),
            NULL);
    if (!reply) return 1;

    modmap = xcb_get_modifier_mapping_keycodes(reply);
    if (!modmap) return 1;

    numlock = xcb_get_keycodes(XK_Num_Lock);
    for (unsigned int i=0; i<8; i++)
       for (unsigned int j=0; j<reply->keycodes_per_modifier; j++) {
           xcb_keycode_t keycode = modmap[i * reply->keycodes_per_modifier + j];
           if (keycode == XCB_NO_SYMBOL) continue;
           for (unsigned int n=0; numlock[n] != XCB_NO_SYMBOL; n++)
               if (numlock[n] == keycode) {
                   numlockmask = 1 << i;
                   break;
               }
       }

    return 0;
}

int main() {
    // Ignore SIGCHILD
    signal(SIGCHLD, SIG_IGN);

    if  (xcb_connection_has_error( dpy = xcb_connect(NULL, NULL) ))
        return 1;

    scr = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;

    if (!scr) {
        xcb_disconnect(dpy);
        return 1;
    }

    if ()
}
