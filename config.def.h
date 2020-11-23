#ifndef CONFIG_H
#define CONFIG_H

#define MOD             XCB_MOD_MASK_4       /* Super/Windows key */
#define CONTROL         XCB_MOD_MASK_CONTROL /* Control key */
#define SHIFT           XCB_MOD_MASK_SHIFT   /* Shift key */

const unsigned int MIN_WINDOW_SIZE    = 100;
const unsigned int WINDOW_RESIZE_DIFF = 5;

const char* menu[]    	= {"rofi", "-show", "drun",	0};
const char* term[]    	= {"kitty",			0};
const char* brwsr[]     = {"firefox", "--new-tab", 	0};
const char* scrotpp[]   = {"scrot++",                   0};
const char* scrotppw[]  = {"scrot++", "-w",		0};
const char* scrotpps[]  = {"scrot++", "-s",		0};
const char* briup[]   	= {"xbacklight", "-inc", "2", 	0};
const char* bridown[] 	= {"xbacklight", "-dec", "2", 	0};
const char* voldown[] 	= {"amixer", "sset", "Master", "2%-",		0};
const char* volup[]   	= {"amixer", "sset", "Master", "2%+",		0};
const char* volmute[] 	= {"amixer", "sset", "Master", "toggle",	0};
const char* status[]	= {"status", 					0};
const char* wallpaper[] = {"wllppr",                                    0};

static struct key keys[] = {
    {MOD,      XK_q,   win_kill,   {0}},
    //{MOD,      XK_c,   win_center, {0}},
    //{MOD,      XK_f,   win_fs,     {0}},

    {XCB_MOD_MASK_1,       XK_Tab, win_next,   {0}},
    {XCB_MOD_MASK_1|SHIFT, XK_Tab, win_prev,   {0}},

    {MOD,       XK_Return, 	run, {.com = term}},
    {MOD,       XK_d,      	run, {.com = menu}},
    {MOD,       XK_w,      	run, {.com = brwsr}},

    {0  ,       XK_Print,	run, {.com = scrotpp}},
    {MOD,       XK_Print,	run, {.com = scrotppw}},
    {MOD|SHIFT, XK_Print,	run, {.com = scrotpps}},

    {MOD,       XK_s,		run, {.com = status}},
    {MOD|SHIFT, XK_w,		run, {.com = wallpaper}},

    {0,   XF86XK_AudioLowerVolume,  run, {.com = voldown}},
    {0,   XF86XK_AudioRaiseVolume,  run, {.com = volup}},
    {0,   XF86XK_AudioMute,         run, {.com = volmute}},
    {0,   XF86XK_MonBrightnessUp,   run, {.com = briup}},
    {0,   XF86XK_MonBrightnessDown, run, {.com = bridown}},

    /*
    {MOD|SHIFT, XK_Left , win_resize_w, {.i = -WINDOW_RESIZE_DIFF}},
    {MOD|SHIFT, XK_Right, win_resize_w, {.i = WINDOW_RESIZE_DIFF}},
    {MOD|SHIFT, XK_Up   , win_resize_h, {.i = -WINDOW_RESIZE_DIFF}},
    {MOD|SHIFT, XK_Down , win_resize_h, {.i = WINDOW_RESIZE_DIFF}},
    */

    {MOD,               XK_1, go_to_ws, {.i = 1}},
    {MOD|SHIFT,         XK_1, mv_to_ws, {.i = 1}},
    {MOD,               XK_2, go_to_ws, {.i = 2}},
    {MOD|SHIFT,         XK_2, mv_to_ws, {.i = 2}},
    {MOD,               XK_3, go_to_ws, {.i = 3}},
    {MOD|SHIFT,         XK_3, mv_to_ws, {.i = 3}},
    {MOD,               XK_4, go_to_ws, {.i = 4}},
    {MOD|SHIFT,         XK_4, mv_to_ws, {.i = 4}},
    {MOD,               XK_5, go_to_ws, {.i = 5}},
    {MOD|SHIFT,         XK_5, mv_to_ws, {.i = 5}},
    {MOD,               XK_6, go_to_ws, {.i = 6}},
    {MOD|SHIFT,         XK_6, mv_to_ws, {.i = 6}},
    {MOD,               XK_7, go_to_ws, {.i = 7}},
    {MOD|SHIFT,         XK_7, mv_to_ws, {.i = 7}},
    {MOD,               XK_8, go_to_ws, {.i = 8}},
    {MOD|SHIFT,         XK_8, mv_to_ws, {.i = 8}},
    {MOD,               XK_9, go_to_ws, {.i = 9}},
    {MOD|SHIFT,         XK_9, mv_to_ws, {.i = 9}},
    {MOD,               XK_0, go_to_ws, {.i = 0}},
    {MOD|SHIFT,         XK_0, mv_to_ws, {.i = 0}},
};

#endif
