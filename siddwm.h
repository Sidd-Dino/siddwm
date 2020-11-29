#include <xcb/xcb.h>
#define LENGTH(x)       (sizeof(x)/sizeof(*x))
#define MAX(X,Y)        ((X) > (Y) ? (X) : (Y))
#define CLEANMASK(mask) (mask & ~(numlockmask|XCB_MOD_MASK_LOCK))

typedef struct {
    const char** com;
    const int i;
} Arg;

struct key {
    unsigned int mod;
    xcb_keysym_t keysym;
    void (*function)(const Arg arg);
    const Arg arg;
};

typedef struct client {
        struct client *next, *prev;
        int f , wx , wy;
        unsigned int ww , wh;
        xcb_window_t w;
} client;

void buttonpress(xcb_generic_event_t *e);
void buttonrelease(xcb_generic_event_t *e);
void configurerequest(xcb_generic_event_t *e);
void keypress(xcb_generic_event_t *e);
void maprequest(xcb_generic_event_t *e);
void mapnotify(xcb_generic_event_t *e);
void destroynotify(xcb_generic_event_t *e);
void enternotify(xcb_generic_event_t *e);
void motionnotify(xcb_generic_event_t *e);

void win_add(xcb_window_t w);
void win_center(const Arg arg);
void win_del(xcb_window_t w , int del_cur);
void win_focus(client *c);
void win_fs(const Arg arg);
void win_kill(const Arg arg);
void win_next(const Arg arg);
void win_prev(const Arg arg);
void win_resize_h(const Arg arg);
void win_resize_w(const Arg arg);

void ws_go(const Arg arg);
void win_to_ws(const Arg arg);

void run(const Arg arg);

xcb_keycode_t* xcb_get_keycodes(xcb_keysym_t keysym);
int setup_keyboard(void);
int grab_input(void);

static int xerror() { return 0; }
