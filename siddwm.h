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

void win_add(xcb_window_t);
void win_center();
void win_del();
void win_focus();
void win_kill();
void win_next();
void win_prev();

void buttonpress(xcb_generic_event_t *);
void buttonrelease(xcb_generic_event_t *);
void configurerequest(xcb_generic_event_t *);
void keypress(xcb_generic_event_t *);
void maprequest(xcb_generic_event_t *);
void mapnotify(xcb_generic_event_t *);
void destroynotify(xcb_generic_event_t *);
void enternotify(xcb_generic_event_t *);
void motionnotify(xcb_generic_event_t *);

void go_to_ws();
void mv_to_ws();

int setup_keyboard();
int setup();

void run(const Arg arg);
void cleanup();

static int xerror() { return 0; }
