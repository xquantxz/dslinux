/*
 * $Id: mousewget.c 1469 2006-10-23 20:59:34Z stsp $
 *
 * mousewget.c -- mouse/wgetch support for dialog
 *
 * Copyright 2000-2003,2005   Thomas E. Dickey
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to
 *	Free Software Foundation, Inc.
 *	51 Franklin St., Fifth Floor
 *	Boston, MA 02110, USA.
 */

#include <dialog.h>
#include <dlg_keys.h>

static int
mouse_wgetch(WINDOW *win, int *fkey, bool ignore_errs)
{
    int key;

#if USE_MOUSE

    do {

	key = dlg_getc(win, fkey);
	if (fkey && (key == KEY_MOUSE)) {
	    MEVENT event;
	    mseRegion *p;

	    if (getmouse(&event) != ERR) {
		if ((p = dlg_mouse_region(event.y, event.x)) != 0) {
		    key = DLGK_MOUSE(p->code);
		} else if ((p = dlg_mouse_bigregion(event.y, event.x)) != 0) {
		    int x = event.x - p->x;
		    int y = event.y - p->y;
		    int row = (p->X - p->x) / p->step_x;

		    key = -(p->code);
		    switch (p->mode) {
		    case 1:	/* index by lines */
			key += y;
			break;
		    case 2:	/* index by columns */
			key += (x / p->step_x);
			break;
		    default:
		    case 3:	/* index by cells */
			key += (x / p->step_x) + (y * row);
			break;
		    }
		} else {
		    (void) beep();
		    key = ERR;
		}
	    } else {
		(void) beep();
		key = ERR;
	    }
	}

    } while (ignore_errs && (key == ERR));

#else

    do {
	key = dlg_getc(win, fkey);
    } while (ignore_errs && (key == ERR));

#endif

    return key;
}

int
dlg_mouse_wgetch(WINDOW *win, int *fkey)
{
    return mouse_wgetch(win, fkey, TRUE);
}

int
dlg_mouse_wgetch_nowait(WINDOW *win, int *fkey)
{
    return mouse_wgetch(win, fkey, FALSE);
}
