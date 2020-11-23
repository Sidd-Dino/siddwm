#include <xcb/xcb.h>
#include <X11/XF86keysym.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "siddwm.h"

static client *list = {0}, *cur;
static xcb_connection_t *dpy;
static xcb_screen_t *screen;
static xcb_window_t root;

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

#include "config.def.h"

void win_add( xcb_window_t win ){
        client *c;
        int w , h ;
        if (!win)
		return;

        if (!(c = (client *) calloc(1, sizeof(client))))
                exit(1);

        c->w = win;

        xcb_get_geometry_reply_t *geom = xcb_get_geometry_reply(dpy,
			xcb_get_geometry(dpy, win), NULL);

        if (geom == NULL)
                return;

        c->ww = geom->width;
	c->wh = geom->height;

        if (list) {

                list->prev->next = c;
                c->prev          = list->prev;
                list->prev       = c;
                c->next          = list;
        } else {
                list = c;
                list->prev = list->next = list;
        }


}

void maprequest(xcb_generic_event_t *ev){
        xcb_map_request_event_t *e = (xcb_map_request_event_t *) ev;

        win_add( e->window );

        cur = list->prev;
}

void run(const Arg arg) {
    if (fork()) return;
    if ( dpy ) close( screen->root );

    setsid();
    execvp((char*)arg.com[0], (char**)arg.com);
}

void cleanup(void)
{
	/* graceful exit */
	if (dpy != NULL)
                xcb_flush(dpy);
		xcb_disconnect(dpy);
}

int main (void){
        atexit(cleanup);

        int Display = 0;

        dpy = xcb_connect(NULL, &Display);
        if (xcb_connection_has_error(dpy)) return 1;

        screen = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;
        root = screen->root;

        xcb_generic_event_t *ev  = NULL;

        setup();

        while ( 1 ) {
                /* the WM is running */
		xcb_flush(dpy);

                if (xcb_connection_has_error(dpy)){
			abort();
        	}

        }
}
