#ifndef CONFIG_H
#define CONFIG_H

#define MOD Mod4Mask
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
        //modifier| Key   | Function  |Argumments
        {MOD,      XK_q,   win_kill,   {0}},
        {MOD,      XK_c,   win_center, {0}},
        {MOD,      XK_f,   win_fs,     {0}},

        {Mod1Mask,           XK_Tab, win_next,   {0}},
        {Mod1Mask|ShiftMask, XK_Tab, win_prev,   {0}},

        {MOD, XK_d,      			run, {.com = menu}},
        {MOD, XK_w,      			run, {.com = brwsr}},
        {0  , XK_Print,			run, {.com = scrotpp}},
        {MOD, XK_Print,			run, {.com = scrotppw}},
        {MOD|ShiftMask, XK_Print,		run, {.com = scrotpps}},
        {MOD, XK_Return, 			run, {.com = term}},
        {MOD, XK_s,				run, {.com = status}},
        {MOD|ShiftMask, XK_w,		run, {.com = wallpaper}},

        {0,   XF86XK_AudioLowerVolume,  run, {.com = voldown}},
        {0,   XF86XK_AudioRaiseVolume,  run, {.com = volup}},
        {0,   XF86XK_AudioMute,         run, {.com = volmute}},
        {0,   XF86XK_MonBrightnessUp,   run, {.com = briup}},
        {0,   XF86XK_MonBrightnessDown, run, {.com = bridown}},

        {MOD|ShiftMask, XK_Up   , win_resize_h, {.i = -WINDOW_RESIZE_DIFF}},
        {MOD|ShiftMask, XK_Down , win_resize_h, {.i = WINDOW_RESIZE_DIFF}},
        {MOD|ShiftMask, XK_Left , win_resize_w, {.i = -WINDOW_RESIZE_DIFF}},
        {MOD|ShiftMask, XK_Right, win_resize_w, {.i = WINDOW_RESIZE_DIFF}},

        {MOD,           XK_1, ws_go,     {.i = 1}},
        {MOD|ShiftMask, XK_1, win_to_ws, {.i = 1}},
        {MOD,           XK_2, ws_go,     {.i = 2}},
        {MOD|ShiftMask, XK_2, win_to_ws, {.i = 2}},
        {MOD,           XK_3, ws_go,     {.i = 3}},
        {MOD|ShiftMask, XK_3, win_to_ws, {.i = 3}},
        {MOD,           XK_4, ws_go,     {.i = 4}},
        {MOD|ShiftMask, XK_4, win_to_ws, {.i = 4}},
        {MOD,           XK_5, ws_go,     {.i = 5}},
        {MOD|ShiftMask, XK_5, win_to_ws, {.i = 5}},
        {MOD,           XK_6, ws_go,     {.i = 6}},
        {MOD|ShiftMask, XK_6, win_to_ws, {.i = 6}},
        {MOD,           XK_7, ws_go,     {.i = 7}},
        {MOD|ShiftMask, XK_7, win_to_ws, {.i = 7}},
        {MOD,           XK_8, ws_go,     {.i = 8}},
        {MOD|ShiftMask, XK_8, win_to_ws, {.i = 8}},
        {MOD,           XK_9, ws_go,     {.i = 9}},
        {MOD|ShiftMask, XK_9, win_to_ws, {.i = 9}},
        {MOD,           XK_0, ws_go,     {.i = 0}},
        {MOD|ShiftMask, XK_0, win_to_ws, {.i = 0}},
};

#endif
