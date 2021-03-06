/*
 *  $Id: inputbox.c 1469 2006-10-23 20:59:34Z stsp $
 *
 *  inputbox.c -- implements the input box
 *
 *  Copyright 2000-2005,2006	Thomas E. Dickey
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
 *
 *  An earlier version of this program lists as authors:
 *	Savio Lam (lam836@cs.cuhk.hk)
 */

#include <dialog.h>
#include <dlg_keys.h>

#define sTEXT -1

/*
 * Display a dialog box for entering a string
 */
int
dialog_inputbox(const char *title, const char *cprompt, int height, int width,
		const char *init, const int password)
{
    /* *INDENT-OFF* */
    static DLG_KEYS_BINDING binding[] = {
	INPUTSTR_BINDINGS,
	ENTERKEY_BINDINGS,
	DLG_KEYS_DATA( DLGK_FIELD_NEXT,	KEY_DOWN ),
	DLG_KEYS_DATA( DLGK_FIELD_NEXT,	KEY_RIGHT ),
	DLG_KEYS_DATA( DLGK_FIELD_NEXT,	TAB ),
	DLG_KEYS_DATA( DLGK_FIELD_PREV,	KEY_BTAB ),
	DLG_KEYS_DATA( DLGK_FIELD_PREV,	KEY_LEFT ),
	DLG_KEYS_DATA( DLGK_FIELD_PREV,	KEY_UP ),
	END_KEYS_BINDING
    };
    /* *INDENT-ON* */

#ifdef KEY_RESIZE
    int old_height = height;
    int old_width = width;
#endif
    int x, y, box_y, box_x, box_width;
    int show_buttons;
    int col_offset = 0;
    int chr_offset = 0;
    int key, fkey, code;
    int result = DLG_EXIT_UNKNOWN;
    int state;
    int first;
    char *input;
    WINDOW *dialog;
    char *prompt = dlg_strclone(cprompt);
    const char **buttons = dlg_ok_labels();

    dlg_does_output();

    dlg_tab_correct_str(prompt);

    /* Set up the initial value */
    input = dlg_set_result(init);

#ifdef KEY_RESIZE
  retry:
#endif
    show_buttons = TRUE;
    state = dialog_vars.defaultno ? dlg_defaultno_button() : sTEXT;
    first = (state == sTEXT);
    key = fkey = 0;

    if (init != NULL) {
	dlg_auto_size(title, prompt, &height, &width, 5,
		      MIN(MAX(dlg_count_columns(init) + 7, 26),
			  SCOLS - (dialog_vars.begin_set ?
				   dialog_vars.begin_x : 0)));
    } else {
	dlg_auto_size(title, prompt, &height, &width, 5, 26);
    }
    dlg_print_size(height, width);
    dlg_ctl_size(height, width);

    x = dlg_box_x_ordinate(width);
    y = dlg_box_y_ordinate(height);

    dialog = dlg_new_window(height, width, y, x);
    dlg_register_window(dialog, "inputbox", binding);
    dlg_register_buttons(dialog, "inputbox", buttons);

    dlg_mouse_setbase(x, y);

    dlg_draw_box(dialog, 0, 0, height, width, dialog_attr, border_attr);
    dlg_draw_bottom_box(dialog);
    dlg_draw_title(dialog, title);

    wattrset(dialog, dialog_attr);
    dlg_print_autowrap(dialog, prompt, height, width);

    /* Draw the input field box */
    box_width = width - 6;
    getyx(dialog, y, x);
    box_y = y + 2;
    box_x = (width - box_width) / 2;
    dlg_mouse_mkregion(y + 1, box_x - 1, 3, box_width + 2, 'i');
    dlg_draw_box(dialog, y + 1, box_x - 1, 3, box_width + 2,
		 border_attr, dialog_attr);

    while (result == DLG_EXIT_UNKNOWN) {
	int edit = 0;

	/*
	 * The last field drawn determines where the cursor is shown:
	 */
	if (show_buttons) {
	    show_buttons = FALSE;
	    col_offset = dlg_edit_offset(input, chr_offset, box_width);
	    (void) wmove(dialog, box_y, box_x + col_offset);
	    dlg_draw_buttons(dialog, height - 2, 0, buttons, state, FALSE, width);
	}

	if (!first) {
	    key = dlg_mouse_wgetch(dialog, &fkey);
	    if (dlg_result_key(key, fkey, &result))
		break;
	}

	/*
	 * Handle mouse clicks first, since we want to know if this is a button,
	 * or something that dlg_edit_string() should handle.
	 */
	if (fkey
	    && is_DLGK_MOUSE(key)
	    && (code = dlg_ok_buttoncode(key - M_EVENT)) >= 0) {
	    result = code;
	    continue;
	}

	if (state == sTEXT) {	/* Input box selected */
	    edit = dlg_edit_string(input, &chr_offset, key, fkey, first);

	    if (edit) {
		dlg_show_string(dialog, input, chr_offset, inputbox_attr,
				box_y, box_x, box_width, password, first);
		first = FALSE;
		continue;
	    } else if (first) {
		first = FALSE;
		continue;
	    }
	}

	/* handle non-functionkeys */
	if (!fkey && (code = dlg_char_to_button(key, buttons)) >= 0) {
	    dlg_del_window(dialog);
	    result = dlg_ok_buttoncode(code);
	    continue;
	}

	/* handle functionkeys */
	if (fkey) {
	    switch (key) {
	    case DLGK_MOUSE('i'):	/* mouse enter events */
		state = 0;
		/* FALLTHRU */
	    case DLGK_FIELD_PREV:
		show_buttons = TRUE;
		state = dlg_prev_ok_buttonindex(state, sTEXT);
		break;
	    case DLGK_FIELD_NEXT:
		show_buttons = TRUE;
		state = dlg_next_ok_buttonindex(state, sTEXT);
		break;
	    case ' ':		/* FIXME: conflict with inputstr.c */
	    case DLGK_ENTER:
		dlg_del_window(dialog);
		result = (state >= 0) ? dlg_ok_buttoncode(state) : DLG_EXIT_OK;
		break;
#ifdef KEY_RESIZE
	    case KEY_RESIZE:
		/* reset data */
		height = old_height;
		width = old_width;
		/* repaint */
		dlg_clear();
		dlg_del_window(dialog);
		refresh();
		dlg_mouse_free_regions();
		goto retry;
#endif
	    default:
		beep();
		break;
	    }
	} else {
	    beep();
	}
    }

    dlg_del_window(dialog);
    dlg_mouse_free_regions();
    free(prompt);
    return result;
}
