/*
 * Modified special keys.
 *
 * The devdraw keyboard protocol carries one rune per keystroke and no
 * modifier state, so Shift+Left arrives as Kleft. When the environment
 * variable DEVDRAW_MODKEYS is set (and not "0"), a special key pressed
 * with Shift, Control, Alt/Option or Cmd/Super is instead reported as
 *
 *	Kmod | mods<<5 | key
 *
 * with mods a combination of Kmodshift, Kmodctl, Kmodalt and Kmodcmd and
 * key the index from modkeyindex (1 Left, 2 Right, 3 Up, 4 Down, 5 Home,
 * 6 End, 7 PgUp, 8 PgDn, 9 Ins, 10 Del, 11 Backspace, 12 Tab, 13 Enter,
 * 14 Esc). Without the variable nothing changes, so clients that do not
 * know these runes never see them.
 */

enum {
	Kmodshift = 1,
	Kmodctl = 2,
	Kmodalt = 4,
	Kmodcmd = 8,
	Kmodmax = Kmod | (15<<5) | 31,
};

static int
modkeyindex(int k)
{
	switch(k){
	case Kleft: return 1;
	case Kright: return 2;
	case Kup: return 3;
	case Kdown: return 4;
	case Khome: return 5;
	case Kend: return 6;
	case Kpgup: return 7;
	case Kpgdown: return 8;
	case Kins: return 9;
	case Kdel: return 10;
	case Kbs: return 11;
	case '\t': return 12;
	case '\n': return 13;
	case Kesc: return 14;
	}
	return 0;
}

/* modkey returns the rune to send for key k with modifiers mods, or k
 * itself when k is not a special key or mods is empty. */
static int
modkey(int k, int mods)
{
	int i;

	if(mods == 0)
		return k;
	i = modkeyindex(k);
	if(i == 0)
		return k;
	return Kmod | (mods<<5) | i;
}

static int
modkeysenabled(void)
{
	static int enabled = -1;
	char *s;

	if(enabled < 0){
		s = getenv("DEVDRAW_MODKEYS");
		enabled = s != nil && *s != '\0' && strcmp(s, "0") != 0;
	}
	return enabled;
}
