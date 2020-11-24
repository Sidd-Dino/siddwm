#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <X11/XF86keysym.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "siddwm.h"

static client *list = {0}, *ws_list[WORKSPACES], *cur;
static xcb_connection_t *dpy;
static xcb_screen_t *screen;
static xcb_window_t root;
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

#include "config.def.h"

void
cleanup(){
        struct client *curr,*wsitem;
        for(int i = 0; i < WORKSPACES; i++){
                if(!ws_list[i])
                continue;
                curr = ws_list[i];
                while(curr){
                        wsitem = curr;
                        curr = curr->next;
                        free(wsitem);
                }
        }

        if(!dpy){
                return;
        }

        xcb_flush(dpy);
	xcb_disconnect(dpy);
}

void
run(const Arg *arg){
	if (fork())
		return;

	if (dpy)
		close(screen->root);

	setsid();
	execvp((char*)arg->com[0], (char**)arg->com);
}

xcb_keycode_t *
xcb_get_keycodes(xcb_keysym_t keysym)
{
	xcb_key_symbols_t *keysyms;
	xcb_keycode_t *keycode;

	if (!(keysyms = xcb_key_symbols_alloc(dpy)))
		return NULL;

	keycode = xcb_key_symbols_get_keycode(keysyms, keysym);
	xcb_key_symbols_free(keysyms);

	return keycode;
}

int
grab_input(void){
        xcb_get_modifier_mapping_reply_t *reply;
	xcb_keycode_t *modmap, *numlock ,*keycode;
        xcb_key_symbols_t *keysyms;
	unsigned int i,j,n ,modifiers[] = {
		0,
		XCB_MOD_MASK_LOCK,
		numlockmask,
		numlockmask | XCB_MOD_MASK_LOCK
	};

	reply = xcb_get_modifier_mapping_reply(dpy,
				xcb_get_modifier_mapping_unchecked(dpy), NULL);

	if (!reply)
		return 0;

	modmap = xcb_get_modifier_mapping_keycodes(reply);

	if (!modmap)
		return 0;

        if (!(keysyms = xcb_key_symbols_alloc(dpy)))
		return 0;

	numlock = xcb_get_keycodes(XK_Num_Lock);

        for ( i=0 ; i<8 ; i++ )
                for (j=0; j<reply->keycodes_per_modifier; j++) {
                        xcb_keycode_t keycode = modmap[
                                i * reply->keycodes_per_modifier + j
                        ];
                        if( keycode != XCB_NO_SYMBOL && numlock != NULL)
                                for (n=0; numlock[n] != XCB_NO_SYMBOL; n++)
                                        if (numlock[n] == keycode) {
                                                numlockmask = 1 << i;
                                                break;
                                        }


                }
        free(reply);
	free(numlock);

        xcb_ungrab_key(dpy, XCB_GRAB_ANY, screen->root, XCB_MOD_MASK_ANY);

        for (i=0; i<LENGTH(keys); i++) {
                keycode = xcb_get_keycodes(keys[i].keysym);
                for (j=0; keycode[k] != XCB_NO_SYMBOL; j++)
			for (n=0; n<LENGTH(modifiers); n++)
				xcb_grab_key(dpy, 1, root, keys[i].mod
					| modifiers[n], keycode[j],
					XCB_GRAB_MODE_ASYNC,//pointer mode
					XCB_GRAB_MODE_ASYNC //keyboard mode
				);
		free(keycode); // allocated in xcb_get_keycodes()
        }

        for (i=1;i<4;i+=2)
                for(j=0;j<LENGTH(modifiers);j++)
                        xcb_grab_button(dpy, 1, root,
                                XCB_EVENT_MASK_BUTTON_PRESS|
                                XCB_EVENT_MASK_BUTTON_RELEASE|
                                XCB_EVENT_MASK_POINTER_MOTION,
                                XCB_GRAB_MODE_ASYNC,XCB_GRAB_MODE_ASYNC,
                                XCB_NONE, XCB_NONE, , MOD|modifiers[j])
}

int
main () {
        atexit(cleanup);

        int Display = 0;

        dpy = xcb_connect(NULL, &Display);
        if (xcb_connection_has_error(dpy)) return 1;

        screen = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;
        root = screen->root;

         if (grab_input()) return 1;

}
