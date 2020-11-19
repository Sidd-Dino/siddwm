# dinowm (Dino's Window Manager*)

A fork of [sowm](https://github.com/dylanaraps/sowm) with an added keybinding. That's all

- Floating only.
- Fullscreen toggle.
- Window centering.
- Mix of mouse and keyboard workflow.
- Focus with cursor .
- Alt-Tab window focusing.
- All windows die on exit.
- No window borders.
- [No ICCCM](https://web.archive.org/web/20190617214524/https://raw.githubusercontent.com/kfish/xsel/1a1c5edf0dc129055f7764c666da2dd468df6016/rant.txt).
- No EWMH.
- etc etc etc

## Default Keybindings

**Window Management**

| combo                        | action                 |
| -----------------------------| -----------------------|
| `Mouse`                      | focus under cursor     |
| `MOD4` + `Left Mouse`        | move window            |
| `MOD4` + `Right Mouse`       | resize window          |
| `MOD4` + `Right/Left`        | inc/dec window width   |
| `MOD4` + `Shift` + `Down/Up` | inc/dec window height  |
| `MOD4` + `f`                 | maximize toggle        |
| `MOD4` + `c`                 | center window          |
| `MOD4` + `q`                 | kill window            |
| `MOD4` + `1-9`               | desktop swap           |
| `MOD4` + `Shift` +`1-9`      | send window to desktop |
| `MOD1` + `TAB` (*alt-tab*)   | focus cycle            |

**Programs**

| combo                    | action           | program        |
| ------------------------ | ---------------- | -------------- |
| `MOD4` + `Return`        | terminal         | `kitty`        |
| `MOD4` + `d`             | dmenu            | `rofi`         |
| `MOD4` + `p`             | scrot            | `i3-scrot`     |
| `MOD4` + `w`             | wallpaper cycler | `wllppr`       |
| `XF86_AudioLowerVolume`  | volume down      | `amixer`       |
| `XF86_AudioRaiseVolume`  | volume up        | `amixer`       |
| `XF86_AudioMute`         | volume toggle    | `amixer`       |
| `XF86_MonBrightnessUp`   | brightness up    | `bri`          |
| `XF86_MonBrightnessDown` | brightness down  | `bri`          |


## Dependencies

- `xlib` (*usually `libX11`*).


## Installation

1) Copy `config.def.h` to `config.h` and modify it to suit your needs.
2) Run `make` to build `dinowm`.
3) Copy it to your path or run `make install`.
    - `DESTDIR` and `PREFIX` are supported.
4) (Optional) Apply patch with `git apply patches/patch-name`
    - In case of applying multiple patches, it has to be done **manually**.

If you are using GDM, save the following to `/usr/share/xsessions/dinowm.desktop`. It is still recommended to start `sowm` from `.xinitrc` or through
[your own xinit implementation](https://github.com/dylanaraps/bin/blob/dfd9a9ff4555efb1cc966f8473339f37d13698ba/x).

```
[Desktop Entry]
Name=dinowm
Comment=This session runs sowm as desktop manager
Exec=dinowm
Type=Application
```


## Thanks

- [sowm](https://github.com/dylanaraps/sowm)
- [2bwm](https://github.com/venam/2bwm)
- [SmallWM](https://github.com/adamnew123456/SmallWM)
- [berry](https://github.com/JLErvin/berry)
- [catwm](https://github.com/pyknite/catwm)
- [dminiwm](https://github.com/moetunes/dminiwm)
- [dwm](https://dwm.suckless.org)
- [monsterwm](https://github.com/c00kiemon5ter/monsterwm)
- [openbox](https://github.com/danakj/openbox)
- [possum-wm](https://github.com/duckinator/possum-wm)
- [swm](https://github.com/dcat/swm)
- [tinywm](http://incise.org/tinywm.html)
