#ifndef CONFIG_H
#define CONFIG_H

#define MOD             XCB_MOD_MASK_4       /* Super/Windows key */
#define CONTROL         XCB_MOD_MASK_CONTROL /* Control key */
#define SHIFT           XCB_MOD_MASK_SHIFT   /* Shift key */

static struct key keys[] = {
    {MOD,      XK_q,   win_kill,   {0}},
    {MOD,      XK_c,   win_center, {0}},
    {MOD,      XK_f,   win_fs,     {0}},

    {XCB_MOD_MASK_1,       XK_Tab, win_next,   {0}},
    {XCB_MOD_MASK_1|SHIFT, XK_Tab, win_prev,   {0}},


    {MOD,               XK_1, ws_go, {.i = 1}},
    {MOD|SHIFT,         XK_1, win_to_ws, {.i = 1}},
    {MOD,               XK_2, ws_go, {.i = 2}},
    {MOD|SHIFT,         XK_2, win_to_ws, {.i = 2}},
    {MOD,               XK_3, ws_go, {.i = 3}},
    {MOD|SHIFT,         XK_3, win_to_ws, {.i = 3}},
    {MOD,               XK_4, ws_go, {.i = 4}},
    {MOD|SHIFT,         XK_4, win_to_ws, {.i = 4}},
    {MOD,               XK_5, ws_go, {.i = 5}},
    {MOD|SHIFT,         XK_5, win_to_ws, {.i = 5}},
    {MOD,               XK_6, ws_go, {.i = 6}},
    {MOD|SHIFT,         XK_6, win_to_ws, {.i = 6}},
    {MOD,               XK_7, ws_go, {.i = 7}},
    {MOD|SHIFT,         XK_7, win_to_ws, {.i = 7}},
    {MOD,               XK_8, ws_go, {.i = 8}},
    {MOD|SHIFT,         XK_8, win_to_ws, {.i = 8}},
    {MOD,               XK_9, ws_go, {.i = 9}},
    {MOD|SHIFT,         XK_9, win_to_ws, {.i = 9}},
    {MOD,               XK_0, ws_go, {.i = 0}},
    {MOD|SHIFT,         XK_0, win_to_ws, {.i = 0}},

};

#endif
