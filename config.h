/* modifier 0 means no modifier */
//#define HOMEPAGE "https://yandex.com/"
#define DOWNLOADS "~/Downloads"
#define HS_FILE "~/.config/surf/history"
#define BM_FILE "~/.config/surf/bookmarks"

static int  surfuseragent   = 1;  /* Append Surf version to default WebKit user agent */
static char *fulluseragent  = ""; /* Or override the whole user agent string */
static char *scriptfile     = "~/.config/surf/script.js";
static char *styledir       = "~/.config/surf/styles/";
static char *certdir        = "~/.config/surf/certificates/";
static char *cachedir       = "~/.config/surf/cache/";
static char *cookiefile     = "~/.config/surf/cookies.txt";

static char *bookmarkfile   = BM_FILE;
static char *historyfile    = HS_FILE;

static char *searchengine = "https://yandex.com/search/?text=";

static SearchEngine searchengines[] = {
    { "wt", "https://www.wiktionary.org/search-redirect.php?family=wiktionary&search=%s" },
    { "aw", "https://wiki.archlinux.org/index.php?search=%s" },
    { "yt", "https://www.youtube.com/results?search_query=%s" },
};

/* Webkit default features */
/* Highest priority value will be used.
 * Default parameters are priority 0
 * Per-uri parameters are priority 1
 * Command parameters are priority 2
 */
static Parameter defconfig[ParameterLast] = {
	/* parameter                    Arg value       priority */
	[AccessMicrophone]    =       { { .i = 0 },     },
	[AccessWebcam]        =       { { .i = 0 },     },
	[Certificate]         =       { { .i = 0 },     },
	[CaretBrowsing]       =       { { .i = 0 },     },
	[CookiePolicies]      =       { { .v = "@Aa" }, },
	[DarkMode]            =       { { .i = 1 },     },
	[DefaultCharset]      =       { { .v = "UTF-8" }, },
	[DiskCache]           =       { { .i = 1 },     },
	[DNSPrefetch]         =       { { .i = 0 },     },
	[Ephemeral]           =       { { .i = 0 },     },
	[FileURLsCrossAccess] =       { { .i = 0 },     },
	[FontSize]            =       { { .i = 12 },    },
	[FrameFlattening]     =       { { .i = 0 },     },
	[Geolocation]         =       { { .i = 0 },     },
	[HideBackground]      =       { { .i = 0 },     },
	[Inspector]           =       { { .i = 0 },     },
	[Java]                =       { { .i = 0 },     },
	[JavaScript]          =       { { .i = 0 },     },
	[KioskMode]           =       { { .i = 0 },     },
	[LoadImages]          =       { { .i = 1 },     },
	[MediaManualPlay]     =       { { .i = 1 },     },
	[PreferredLanguages]  =       { { .v = (char *[]){ NULL } }, },
	[RunInFullscreen]     =       { { .i = 0 },     },
	[ScrollBars]          =       { { .i = 0 },     },
	[ShowIndicators]      =       { { .i = 0 },     },
	[SiteQuirks]          =       { { .i = 1 },     },
	[SmoothScrolling]     =       { { .i = 0 },     },
	[SpellChecking]       =       { { .i = 0 },     },
	[SpellLanguages]      =       { { .v = ((char *[]){ "en_US", NULL }) }, },
	[StrictTLS]           =       { { .i = 1 },     },
	[Style]               =       { { .i = 1 },     },
	[WebGL]               =       { { .i = 0 },     },
	[ZoomLevel]           =       { { .f = 0.8 },   },
	[ClipboardNotPrimary] =	      { { .i = 1 },	},
};

static UriParameters uriparams[] = {
	{ "(://|\\.)(github\\.com|gitlab\\.com)(/|$)", {
	  [JavaScript] = { { .i = 1 }, 1 },
	}, },
	{ "(://|\\.)(youtube\\.com|localhost:9091)(/|$)", {
	  [JavaScript] = { { .i = 1 }, 1 },
          [Style] = { { .i = 0 }, 1 },
	}, },
	{ "(://|\\.)suckless\\.org(/|$)", {
	  [JavaScript] = { { .i = 0 }, 1 },
	}, },
};

/* default window size: width, height */
static int winsize[] = { 800, 600 };

static WebKitFindOptions findopts = WEBKIT_FIND_OPTIONS_CASE_INSENSITIVE |
                                    WEBKIT_FIND_OPTIONS_WRAP_AROUND;

#define PROMPT_GO   "Go:"
#define PROMPT_NEW  "Go: (new window)"
#define PROMPT_FIND "Find:"

/* SETPROP(readprop, setprop, prompt)*/
#define SETPROP(r, s, p) { \
        .v = (const char *[]){ "/bin/sh", "-c", \
             "WIDTH=\"$(xwininfo -id $1 | awk '/Width/{print int($2/5/3)}')\"; " \
             "HIST=\"$(tac " HS_FILE " 2>/dev/null | awk -F'|' '!a[$2]++')\"; " \
             "if [ \"${WIDTH}\" -lt 30 ]; then " \
             "  HIST_TABLE=\"$(printf '%s\n' \"${HIST}\" | awk -F'|' '{print NR\"|\"$3}')\";" \
             "elif [ \"${WIDTH}\" -lt 50 ]; then" \
             "  HIST_TABLE=\"$(printf '%s\n' \"${HIST}\" | awk -F'|' '{if(length($2) > '\"$WIDTH\"'){print NR\"|\"substr($2,0,'\"$WIDTH\"')\"...|\"$3}else{print NR\"|\"$2\"|\"$3}}')\"; " \
             "else HIST_TABLE=\"$(printf '%s\n' \"${HIST}\" | awk -F'|' '{if(length($2) > '\"$WIDTH\"'){print NR\"|\"$1\"|\"substr($2,0,'\"$WIDTH\"')\"...|\"$3}else{print NR\"|\"$1\"|\"$2\"|\"$3}}')\"; fi; " \
             "prop=\"$(printf '%b' \"$(xprop -id $1 "r" " \
             "| sed -e 's/^"r"(UTF8_STRING) = \"\\(.*\\)\"/\\1/' " \
             "      -e 's/\\\\\\(.\\)/\\1/g' "\
             "      && [ \""p"\" != \""PROMPT_FIND"\" ]" \
             "      && cat " BM_FILE " 2>/dev/null " \
             "      && printf '%s\n' \"${HIST_TABLE}\" | column -t -s '|' -o ' ')\" " \
             "| dmenu -i -b -l 20 -p '"p"' -w $1 | awk '{if(length($3)!=0){print $1}else{print $0}}')\"; " \
             "[ \"$prop\" ] || exit; " \
             "[ \"$(echo \"$prop\" | grep -Eo '^\-?[0-9]+$')\" -gt 0 ] && prop=\"$(printf '%s\n' \"${HIST}\" | sed \"${prop}q;d\" | awk -F'|' '{print $2}')\"; " \
             "if [ \""p"\" != \""PROMPT_NEW"\" ]; then xprop -id $1 -f "s" 8u -set "s" \"$prop\"; else surf-open \"$prop\"; fi", \
             "surf-setprop", winid, NULL \
        } \
}

/* DOWNLOAD(URI, referer) */
#define DOWNLOAD(u, r) { \
        .v = (const char *[]){ "st", "-c", "st-float", "-e", "/bin/sh", "-c",\
             "cd " DOWNLOADS ";" \
             "curl -g -L -J -O -A \"$1\" -b \"$2\" -c \"$2\"" \
             " -e \"$3\" \"$4\"; read", \
             "surf-download", useragent, cookiefile, r, u, NULL \
        } \
}

/* PLUMB(URI) */
/* This called when some URI which does not begin with "about:",
 * "http://" or "https://" should be opened.
 */
#define PLUMB(u) {\
        .v = (const char *[]){ "/bin/sh", "-c", \
             "xdg-open \"$0\"", u, NULL \
        } \
}

/* VIDEOPLAY(URI) */
#define VIDEOPLAY(u) {\
        .v = (const char *[]){ "/bin/sh", "-c", \
             "mpv \"$0\"", u, NULL \
        } \
}

/* BM_ADD(readprop) */
#define BM_ADD(r) {\
        .v = (const char *[]){ "/bin/sh", "-c", \
             "(echo $(xprop -id $0 $1) | cut -d '\"' -f2 " \
             "| sed 's/.*https*:\\/\\/\\(www\\.\\)\\?//' && cat ~/.config/surf/bookmarks) " \
             "| awk '!seen[$0]++' > ~/.config/surf/bookmarks.tmp && " \
             "mv ~/.config/surf/bookmarks.tmp ~/.config/surf/bookmarks", \
             winid, r, NULL \
        } \
}

/* styles */
/*
 * The iteration will stop at the first match, beginning at the beginning of
 * the list.
 */
static SiteSpecific styles[] = {
	/* regexp               file in $styledir */
	{ ".*",                 "default.css" },
        //{ ".*suckless.org.*",   "default.css" },
        //{ ".*yandex.com.*",   "default.css" },
};

/* certificates */
/*
 * Provide custom certificate for urls
 */
static SiteSpecific certs[] = {
	/* regexp               file in $certdir */
	{ "://suckless\\.org/", "suckless.org.crt" },
};

#define MODKEY GDK_CONTROL_MASK

static char *editscreen[] = { "/bin/sh", "-c", "surf-edit-source", NULL };
static char *linkselect_curwin [] = { "/bin/sh", "-c",
    "surf-link-select $0 'Link:' | xargs -r xprop -id $0 -f _SURF_GO 8u -set _SURF_GO $1",
    winid, NULL
};
static char *linkselect_newwin [] = { "/bin/sh", "-c",
    "surf-link-select $0 'Link: (new window)' | xargs -r surf-open",
    winid, NULL
};
static char *linkselect_open [] = { "/bin/sh", "-c",
    "surf-link-select $0 'Link: (file handler)' | xargs -r xdg-open",
    winid, NULL
};

/* hotkeys */
/*
 * If you use anything else but MODKEY and GDK_SHIFT_MASK, don't forget to
 * edit the CLEANMASK() macro.
 */
static Key keys[] = {
	/* modifier              keyval          function    arg */
	{ MODKEY,                GDK_KEY_g,      spawn,      SETPROP("_SURF_URI", "_SURF_GO", PROMPT_GO) },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_g,      spawn,      SETPROP("_SURF_URI", "_SURF_GO", PROMPT_NEW) },
	{ MODKEY,                GDK_KEY_Return, spawn,      SETPROP("_SURF_URI", "_SURF_GO", PROMPT_GO) },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_Return, spawn,      SETPROP("_SURF_URI", "_SURF_GO", PROMPT_NEW) },
	{ MODKEY,                GDK_KEY_f,      spawn,      SETPROP("_SURF_FIND", "_SURF_FIND", PROMPT_FIND) },
	{ MODKEY,                GDK_KEY_slash,  spawn,      SETPROP("_SURF_FIND", "_SURF_FIND", PROMPT_FIND) },
	{ MODKEY,                GDK_KEY_m,      spawn,      BM_ADD("_SURF_URI") },

    { MODKEY,                GDK_KEY_e,      externalpipe, { .v = editscreen } },
    { MODKEY,                GDK_KEY_o,      externalpipe, { .v = linkselect_curwin } },
    { MODKEY|GDK_SHIFT_MASK, GDK_KEY_o,      externalpipe, { .v = linkselect_newwin } },

	{ MODKEY,                GDK_KEY_w,      playexternal, { 0 } },
    { MODKEY|GDK_SHIFT_MASK, GDK_KEY_w,      externalpipe, { .v = linkselect_open } },

	{ 0,                     GDK_KEY_Escape, stop,       { 0 } },
	{ MODKEY,                GDK_KEY_c,      stop,       { 0 } },

	{ GDK_SHIFT_MASK,        GDK_KEY_F5,     reload,     { .i = 1 } },
	{ 0,			         GDK_KEY_F5,     reload,     { .i = 0 } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_r,      reload,     { .i = 1 } },
	{ MODKEY,                GDK_KEY_r,      reload,     { .i = 0 } },

	{ MODKEY,                GDK_KEY_l,      navigate,   { .i = +1 } },
	{ MODKEY,                GDK_KEY_h,      navigate,   { .i = -1 } },

	/* vertical and horizontal scrolling, in viewport percentage */
	{ MODKEY,                GDK_KEY_j,      scrollv,    { .i = +10 } },
	{ MODKEY,                GDK_KEY_k,      scrollv,    { .i = -10 } },
	{ MODKEY,                GDK_KEY_space,  scrollv,    { .i = +50 } },
	{ MODKEY,                GDK_KEY_b,      scrollv,    { .i = -50 } },
	{ MODKEY,                GDK_KEY_i,      scrollh,    { .i = +10 } },
	{ MODKEY,                GDK_KEY_u,      scrollh,    { .i = -10 } },


	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_j,      zoom,       { .i = -1 } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_k,      zoom,       { .i = +1 } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_q,      zoom,       { .i = 0  } },
	{ MODKEY,                GDK_KEY_minus,  zoom,       { .i = -1 } },
	{ MODKEY,                GDK_KEY_plus,   zoom,       { .i = +1 } },

	{ MODKEY,                GDK_KEY_p,      clipboard,  { .i = 1 } },
	{ MODKEY,                GDK_KEY_y,      clipboard,  { .i = 0 } },

	{ MODKEY,                GDK_KEY_n,      find,       { .i = +1 } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_n,      find,       { .i = -1 } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_p,      print,      { 0 } },
	{ MODKEY,                GDK_KEY_t,      showcert,   { 0 } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_a,      togglecookiepolicy, { 0 } },
	{ 0,                     GDK_KEY_F11,    togglefullscreen, { 0 } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_o,      toggleinspector, { 0 } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_c,      toggle,     { .i = CaretBrowsing } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_f,      toggle,     { .i = FrameFlattening } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_g,      toggle,     { .i = Geolocation } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_s,      toggle,     { .i = JavaScript } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_i,      toggle,     { .i = LoadImages } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_b,      toggle,     { .i = ScrollBars } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_t,      toggle,     { .i = StrictTLS } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_d,      toggle,     { .i = DarkMode } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_m,      toggle,     { .i = Style } },
};

/* button definitions */
/* target can be OnDoc, OnLink, OnImg, OnMedia, OnEdit, OnBar, OnSel, OnAny */
static Button buttons[] = {
	/* target       event mask      button  function        argument        stop event */
	{ OnLink,       0,              2,      clicknewwindow, { .i = 0 },     1 },
	{ OnLink,       MODKEY,         2,      clicknewwindow, { .i = 1 },     1 },
	{ OnLink,       MODKEY,         1,      clicknewwindow, { .i = 1 },     1 },
	{ OnAny,        0,              8,      clicknavigate,  { .i = -1 },    1 },
	{ OnAny,        0,              9,      clicknavigate,  { .i = +1 },    1 },
	{ OnMedia,      MODKEY,         1,      clickexternplayer, { 0 },       1 },
};

