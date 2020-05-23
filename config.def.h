/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
/* static const char *fonts[]          = { */
/*   "Mononoki Nerd Font:size=12:antialias=true:autohint=true", */
/*   /\* "FiraCode Nerd Font Mono:size=12", *\/ */
/*   "JoyPixels:size=12:antialias=true:autohint=true", */
/*   "Hack:size=12:antialias=true:autohint=true", */
/* }; */
static const char *fonts[]          = {
  "FiraCode Nerd Font Mono:size=12", //"monospace:size=10",
  "JoyPixels:pixelsize=10:antialias=true:autohint=true",
};
static const char dmenufont[]       = "FiraCode Nerd Font Mono:size=14";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",              NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",           NULL,       NULL,       1 << 8,       0,           -1 },
	{ "jetbrains-goland",  NULL,       "win0",       0,       1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "HHH",      grid },
  { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_Up,    ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_Down,  ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	{ MOD, XK_x,     ACTION##stack, {.i = -1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
/* static const char *termcmd[] = {"st", NULL}; */
/* static const char *termcmd[] = {"alacritty", NULL}; */
static const char *termcmd[] = {"termite", NULL};
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "termite", "-t", scratchpadname, "-g", "120x34", NULL };
static const char *toggleExternalMonitor[] = {"toggle-external-monitor-dwm", NULL};
static const char *emacs[] = {"emacs", NULL};
/* static const char *sublime[] = {"subl", NULL}; */
static const char *code[] = {"code", NULL};
/* static const char *brave[] = {"brave", "--force-dark-mode", "--ignore-gpu-blacklist", "--enable-gpu-rasterization", "--enable-native-gpu-memory-buffers", "--enable-zero-copy", "--enable-accelerated-mjpeg-decode", "--enable-accelerated-video", "--password-store=gnome", NULL}; */
static const char *chrome[] = {"google-chrome-stable", NULL };
/* static const char *firefox[] = {"firefox", NULL }; */
static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };
static const char *upbright[] = { "/usr/bin/xbacklight",   "-inc",    "5", NULL };
static const char *downbright[] = { "/usr/bin/xbacklight", "-dec",    "5", NULL };
static const char *wally[] = { "/bin/wally", NULL };
static const char *maimsel[] = { "maimsel", NULL };
static const char *goland[] = { "/home/jim/.local/bin/tools/goland", NULL };

/* #include "shiftview.c" */

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
  { MODKEY|ShiftMask,             XK_Up,     rotatestack,    {.i = +1 } },
  { MODKEY|ShiftMask,             XK_Down,   rotatestack,    {.i = -1 } },
  STACKKEYS(MODKEY,                          focus)
  STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY|ControlMask,           XK_Up,  cyclelayout,       {.i = -1 } },
	{ MODKEY|ControlMask,           XK_Down, cyclelayout,      {.i = +1 } },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_Page_Up,   focusmon,    {.i = -1 } },
	{ MODKEY,                       XK_Page_Down, focusmon,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Page_Up,   tagmon,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Page_Down, tagmon,      {.i = +1 } },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
  { 0,                            XK_Print,    spawn,          {.v = maimsel} },
  { ControlMask,                  XK_F10,    spawn,          {.v = goland} },
  { ControlMask,                  XK_F9,     spawn,          {.v = chrome} },
  { ControlMask,                  XK_F11,    spawn,          {.v = code} },
  { ControlMask,                  XK_F12,    spawn,          {.v = emacs} },
  { MODKEY,                       XK_Right,  shiftview,      { .i = +1 } },
	{ MODKEY,                       XK_Left,   shiftview,      { .i = -1 } },
  { 0,                            XF86XK_AudioLowerVolume,   spawn,    {.v = downvol } },
	{ 0,                            XF86XK_AudioMute,          spawn,    {.v = mutevol } },
	{ 0,                            XF86XK_AudioRaiseVolume,   spawn,    {.v = upvol   } },
  { 0,                            XF86XK_MonBrightnessUp,    spawn,  {.v = upbright } },
  { 0,                            XF86XK_MonBrightnessDown,  spawn,  {.v = downbright } },
  { MODKEY,                       XK_F8,     spawn,          {.v = wally} },
  { MODKEY,                       XK_F7,     spawn,          {.v = toggleExternalMonitor} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_BackSpace, quit,        {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

