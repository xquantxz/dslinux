//
// "$Id: Fl_Menu.cxx 1425 2006-10-03 11:27:15Z dslinux_amadeus $"
//
// Menu code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-1999 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@easysw.com".
//

// Warning: this menu code is quite a mess!

// This file contains code for implementing Fl_Menu_Item, and for
// methods for bringing up popup menu hierarchies without using the
// Fl_Menu_ widget.

#include <FL/Fl.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Menu_.H>
#include <stdio.h>
#include <FL/fl_draw.H>

int
Fl_Menu_Item::size() const
{
    const Fl_Menu_Item *m = this;
    int nest = 0;
    for (;;) {
	if (!m->text) {
	    if (!nest)
		return (m - this + 1);
	    nest--;
	} else if (m->flags & FL_SUBMENU) {
	    nest++;
	}
	m++;
    }
}

const Fl_Menu_Item *
Fl_Menu_Item::next(int n) const
{
    if (n < 0)
	return 0;		// this is so selected==-1 returns NULL
    const Fl_Menu_Item *m = this;
    int nest = 0;
    while (n > 0) {
	if (!m->text) {
	    if (!nest)
		return m;
	    nest--;
	} else if (m->flags & FL_SUBMENU) {
	    nest++;
	}
	m++;
	if (!nest && m->visible())
	    n--;
    }
    return m;
}

// appearance of current menus are pulled from this parent widget:
static const Fl_Menu_ *button;

////////////////////////////////////////////////////////////////

// tiny window for title of menu:
class menutitle:public Fl_Menu_Window
{
    void draw();
  public:
    const Fl_Menu_Item *menu;
      menutitle(int X, int Y, int W, int H, const Fl_Menu_Item *);
};

// each vertical menu has one of these:
class menuwindow:public Fl_Menu_Window
{
    void draw();
    void drawentry(const Fl_Menu_Item *, int i, int erase);
  public:
      menutitle * title;
    int handle(int);
    int itemheight;		// zero == menubar
    int numitems;
    int selected;
    int drawn_selected;		// last redraw has this selected
    const Fl_Menu_Item *menu;
      menuwindow(const Fl_Menu_Item * m, int X, int Y, int W, int H,
		 const Fl_Menu_Item * picked, const Fl_Menu_Item * title,
		 int menubar = 0, int menubar_title = 0);
     ~menuwindow();
    void set_selected(int);
    int find_selected(int mx, int my);
    int titlex(int);
    void autoscroll(int);
    void position(int x, int y);
};

#define LEADING 4		// extra vertical leading

extern char fl_draw_shortcut;

// width of label, including effect of & characters:
int
Fl_Menu_Item::measure(int *hp, const Fl_Menu_ * m) const
{
    Fl_Label l;
    l.value = text;
    l.type = labeltype_;
    l.font =
	labelsize_ ? labelfont_ : uchar(m ? m->textfont() : FL_HELVETICA);
    l.size = labelsize_ ? labelsize_ : m ? m->textsize() : FL_NORMAL_SIZE;
    l.color = labelcolor_;
    fl_draw_shortcut = 1;
    int w = 0;
    int h = 0;
    l.measure(w, hp ? *hp : h);
    fl_draw_shortcut = 0;
    if (flags & (FL_MENU_TOGGLE | FL_MENU_RADIO))
	w += 14;
    return w;
}

void
Fl_Menu_Item::draw(int x, int y, int w, int h, const Fl_Menu_ * m,
		   int selected) const
{

    Fl_Label l;
    l.value = text;
    l.type = labeltype_;
    l.font =
	labelsize_ ? labelfont_ : uchar(m ? m->textfont() : FL_HELVETICA);
    l.size = labelsize_ ? labelsize_ : m ? m->textsize() : FL_NORMAL_SIZE;
    l.color = labelcolor_ ? labelcolor_ : m ? m->textcolor() : int (FL_BLACK);
    if (!active())
	l.color = inactive((Fl_Color) l.color);
    Fl_Color color = m ? m->color() : FL_GRAY;

    if (selected) {

	Fl_Color r = m ? m->selection_color() : FL_SELECTION_COLOR;
	Fl_Boxtype b = m && m->down_box()? m->down_box() : FL_FLAT_BOX;

	if (contrast(r, color) != r) {	// back compatability boxtypes
	    if (selected == 2) {	// menu title
		r = color;
		b = m ? m->box() : FL_UP_BOX;
	    } else {
		r = (Fl_Color) (FL_COLOR_CUBE - 1);	// white
		l.color = contrast((Fl_Color) labelcolor_, r);
	    }
	} else {
	    l.color = contrast((Fl_Color) labelcolor_, r);
	}

	if (selected == 2) {	// menu title
	    fl_draw_box(b, x, y, w, h, r);
	    x += 3;
	    w -= 8;
	} else {
	    //fl_draw_box(b, x+1, y-1, w-2, h+2, r);
	    fl_draw_box(b, x + 1, y - 1, w - 1, h + 3, r);
	}
    }


    if (flags & (FL_MENU_TOGGLE | FL_MENU_RADIO)) {

	int y1 = y + (h - 14) / 2;
	fl_color(FL_DARK3);
	if (flags & FL_MENU_RADIO) {
	    fl_line(x + 2, y1 + 7, x + 8, y1 + 1, x + 14, y1 + 7);
	    if (selected) {
		fl_color(color);
		fl_polygon(x + 3, y1 + 7, x + 8, y1 + 2, x + 13, y1 + 7,
			   x + 8, y1 + 12);
	    }
	    fl_color(FL_LIGHT3);
	    fl_line(x + 14, y1 + 7, x + 8, y1 + 13, x + 2, y1 + 7);
	    if (value()) {
		fl_color(FL_BLACK);
		fl_polygon(x + 4, y1 + 7, x + 8, y1 + 3, x + 12, y1 + 7,
			   x + 8, y1 + 11);
	    }
	} else {

	    fl_yxline(x + 3, y1 + 11, y1 + 2, x + 12);

	    if (selected) {
		fl_color(color);
		fl_rectf(x + 4, y1 + 3, 9, 9);
	    }

	    fl_color(FL_LIGHT3);
	    fl_xyline(x + 4, y1 + 12, x + 13, y1 + 3);

	    if (value()) {
		fl_color(FL_BLACK);
		fl_rectf(x + 5, y1 + 4, 7, 7);
	    }

	}
	x += 14;
	w -= 14;
    }

    if (!fl_draw_shortcut)
	fl_draw_shortcut = 1;

    l.draw(x + 3, y, w, h, FL_ALIGN_LEFT);
    fl_draw_shortcut = 0;

}

menutitle::menutitle(int X, int Y, int W, int H, const Fl_Menu_Item * L):
Fl_Menu_Window(X, Y, W, H, 0)
{
    end();
    set_modal();
    clear_border();
    menu = L;
    if (L->labelcolor_)
	clear_overlay();
}

menuwindow::menuwindow(const Fl_Menu_Item * m, int X, int Y, int Wp, int Hp,
		       const Fl_Menu_Item * picked, const Fl_Menu_Item * t,
		       int menubar, int menubar_title)
    :
Fl_Menu_Window(X, Y, Wp, Hp, 0)
{
    end();
    set_modal();
    clear_border();
    menu = m;
    drawn_selected = -1;
    box(button && button->box()? button->box() : FL_UP_BOX);
    color(button ? button->color() : FL_GRAY);
    selected = -1;
    {
	int i = 0;
	if (m)
	    for (const Fl_Menu_Item * m1 = m;; m1 = m1->next(), i++) {
		if (picked) {
		    if (m1 == picked) {
			selected = i;
			picked = 0;
		    } else if (m1 > picked) {
			selected = i - 1;
			picked = 0;
			Wp = Hp = 0;
		    }
		}
		if (!m1->text)
		    break;
	    }
	numitems = i;
    }

    if (menubar) {
	itemheight = 0;
	title = 0;
	return;
    }

    itemheight = 1;

    int hotKeysw = 0;
    int Wtitle = 0;
    int Htitle = 0;
    if (t)
	Wtitle = t->measure(&Htitle, button) + 12;
    int W = 0;
    if (m)
	for (; m->text; m = m->next()) {
	    int h;
	    int w1 = m->measure(&h, button);
	    if (h + LEADING > itemheight)
		itemheight = h + LEADING;
	    if (m->flags & (FL_SUBMENU | FL_SUBMENU_POINTER))
		w1 += 14;
	    if (w1 > W)
		W = w1;
	    if (m->shortcut_) {
		w1 = int (fl_width(fl_shortcut_label(m->shortcut_))) + 8;
		if (w1 > hotKeysw)
		    hotKeysw = w1;
	    }
	    if (m->labelcolor_)
		clear_overlay();
	}
    if (selected >= 0 && !Wp)
	X -= W / 2;
    int BW = Fl::box_dx(box());
    W += hotKeysw + 2 * BW + 7;
    if (Wp > W)
	W = Wp;
    if (Wtitle > W)
	W = Wtitle;

    if (!Wp) {
	if (X < 0)
	    X = 0;
	if (X > Fl::w() - W)
	    X = Fl::w() - W;
    }
    x(X);
    w(W);
    h((numitems ? itemheight * numitems - LEADING : 0) + 2 * BW + 5);
    if (selected >= 0)
	Y = Y + (Hp - itemheight) / 2 - selected * itemheight - BW + 1;
    else
	Y = Y + Hp;
    if (m)
	y(Y - 1);
    else {
	y(Y - 3);
	w(1);
	h(1);
    }

    if (t) {
	int ht = menubar_title ? button->h() - 6 : Htitle + 2 * BW + 3;
	title = new menutitle(X, Y - ht - 3, Wtitle, ht, t);
    } else
	title = 0;
}

menuwindow::~menuwindow()
{
    delete title;
}

void
menuwindow::position(int X, int Y)
{
    if (title) {
	title->position(X, title->y() + Y - y());
    }
    Fl_Menu_Window::position(X, Y);
    // x(X); y(Y); // don't wait for response from X
}

// scroll so item i is visible on screen
void
menuwindow::autoscroll(int i)
{
    int Y = y() + Fl::box_dx(box()) + 2 + i * itemheight;
    if (Y <= Fl::y())
	Y = Fl::y() - Y + 10;
    else {
	Y = Y + itemheight - Fl::h() - Fl::y();
	if (Y < 0)
	    return;
	Y = -Y - 10;
    }
    Fl_Menu_Window::position(x(), y() + Y);
    // y(y()+Y); // don't wait for response from X
}

////////////////////////////////////////////////////////////////

void
menuwindow::drawentry(const Fl_Menu_Item * m, int i, int erase)
{
    if (!m)
	return;			// this happens if -1 is selected item and redrawn

    int BW = Fl::box_dx(box());
    int x = BW;
    int W = this->w();
    int w = W - 2 * BW - 1;
    int y = BW + 2 + i * itemheight;
    int h = itemheight - LEADING;

    if (erase && i != selected) {
	fl_color(button ? button->color() : FL_GRAY);
	//    fl_rectf(x+1, y-1, w-2, h+2);
	fl_rectf(x + 1, y - 1, w - 1, h + 3);
    }

    m->draw(x, y, w, h, button, i == selected);

    // the shortcuts and arrows assumme fl_color() was left set by draw():
    if (m->submenu()) {
	int y1 = y + (h - 14) / 2;
	fl_polygon(x + w - 13, y1 + 2, x + w - 13, y1 + 2 + 10, x + w - 3,
		   y1 + 2 + 5);
    } else if (m->shortcut_) {
	Fl_Font f = button ? button->textfont() : FL_HELVETICA;
	fl_font(f, button ? button->textsize() : FL_NORMAL_SIZE);
	fl_draw(fl_shortcut_label(m->shortcut_), x, y, w - 3, h,
		FL_ALIGN_RIGHT);
    }

    if (m->flags & FL_MENU_DIVIDER) {
	fl_color(FL_DARK3);
	fl_xyline(BW - 1, y + h + 1, W - 2 * BW + 2);
	fl_color(FL_LIGHT3);
	fl_xyline(BW - 1, y + h + 2, W - 2 * BW + 2);
    }

}

void
menutitle::draw()
{
    menu->draw(0, 0, w(), h(), button, 2);
}

void
menuwindow::draw()
{

    if (damage() != FL_DAMAGE_CHILD) {	// complete redraw
	fl_draw_box(box(), 0, 0, w(), h(), color());
	if (menu) {
	    const Fl_Menu_Item *m;
	    int i;
	    for (m = menu, i = 0; m->text; i++, m = m->next())
		drawentry(m, i, 0);
	}
    } else {
	if (damage() & FL_DAMAGE_CHILD && selected != drawn_selected) {	// change selection
	    drawentry(menu->next(drawn_selected), drawn_selected, 1);
	    drawentry(menu->next(selected), selected, 1);
	}
    }
    drawn_selected = selected;
}

void
menuwindow::set_selected(int i)
{
    if (i != selected) {
	selected = i;
	damage(FL_DAMAGE_CHILD);
    }
}

////////////////////////////////////////////////////////////////

int
menuwindow::find_selected(int mx, int my)
{
    if (!menu || !menu->text)
	return -1;
    mx -= x();
    my -= y();
    if (my < 0 || my >= h())
	return -1;
    if (!itemheight) {		// menubar
	int x = 3;
	int i = 0;
	const Fl_Menu_Item *m = menu;
	for (;; m = m->next(), i++) {
	    if (!m->text)
		return -1;
	    x += m->measure(0, button) + 16;
	    if (x > mx)
		break;
	}
	return i;
    }
    if (mx < Fl::box_dx(box()) || mx >= w())
	return -1;
    int i = (my - Fl::box_dx(box()) - 1) / itemheight;
    if (i < 0 || i >= numitems)
	return -1;
    return i;
}

// return horizontal position for item i in a menubar:
int
menuwindow::titlex(int i)
{
    const Fl_Menu_Item *m;
    int x = 3;
    for (m = menu; i--; m = m->next())
	x += m->measure(0, button) + 16;
    return x;
}

// match shortcuts & label shortcuts, don't search submenus:
// returns menu item and index
const Fl_Menu_Item *
Fl_Menu_Item::find_shortcut(int *ip) const
{
    const Fl_Menu_Item *m1 = this;
    for (int ii = 0; m1 && m1->text; m1 = m1->next(1), ii++) {
	if (m1->activevisible() && (Fl::test_shortcut(m1->shortcut_)
				    || Fl_Widget::test_shortcut(m1->text))) {
	    if (ip)
		*ip = ii;
	    return m1;
	}
    }
    return 0;
}

////////////////////////////////////////////////////////////////
// Fl_Menu_Item::popup(...)

// Because Fl::grab() is done, all events go to one of the menu windows.
// But the handle method needs to look at all of them to find out
// what item the user is pointing at.  And it needs a whole lot
// of other state variables to determine what is going on with
// the currently displayed menus.
// So the main loop (handlemenu()) puts all the state in a structure
// and puts a pointer to it in a static location, so the handle()
// on menus can refer to it and alter it.  The handle() method
// changes variables in this state to indicate what item is
// picked, but does not actually alter the display, instead the
// main loop does that.  This is because the X mapping and unmapping
// of windows is slow, and we don't want to fall behind the events.

// values for menustate.state:
#define INITIAL_STATE 0		// no mouse up or down since popup() called
#define PUSH_STATE 1		// mouse has been pushed on a normal item
#define DONE_STATE 2		// exit the popup, the current item was picked
#define MENU_PUSH_STATE 3	// mouse has been pushed on a menu title

struct menustate
{
    const Fl_Menu_Item *current_item;	// what mouse is pointing at
    int menu_number;		// which menu it is in
    int item_number;		// which item in that menu, -1 if none
    menuwindow *p[20];		// pointers to menus
    int nummenus;
    int menubar;		// if true p[0] is a menubar
    int state;
};
static menustate *p;

static inline void
setitem(const Fl_Menu_Item * i, int m, int n)
{
    p->current_item = i;
    p->menu_number = m;
    p->item_number = n;
}

static void
setitem(int m, int n)
{
    menustate & p = *(::p);
    p.current_item = (n >= 0) ? p.p[m]->menu->next(n) : 0;
    p.menu_number = m;
    p.item_number = n;
}

static int
forward(int menu)
{				// go to next item in menu menu if possible
    menustate & p = *(::p);
    menuwindow & m = *(p.p[menu]);
    int item = (menu == p.menu_number) ? p.item_number : m.selected;
    while (++item < m.numitems) {
	const Fl_Menu_Item *m1 = m.menu->next(item);
	if (m1->activevisible()) {
	    setitem(m1, menu, item);
	    return 1;
	}
    }
    return 0;
}

static int
backward(int menu)
{				// previous item in menu menu if possible
    menustate & p = *(::p);
    menuwindow & m = *(p.p[menu]);
    int item = (menu == p.menu_number) ? p.item_number : m.selected;
    if (item < 0)
	item = m.numitems;
    while (--item >= 0) {
	const Fl_Menu_Item *m1 = m.menu->next(item);
	if (m1->activevisible()) {
	    setitem(m1, menu, item);
	    return 1;
	}
    }
    return 0;
}

int
menuwindow::handle(int e)
{
    menustate & p = *(::p);
    switch (e) {
    case FL_KEYBOARD:
	switch (Fl::event_key()) {
	case FL_Tab:
	    if (Fl::event_shift() & FL_SHIFT)
		goto BACKTAB;
	case ' ':
	    if (!forward(p.menu_number)) {
		p.item_number = -1;
		forward(p.menu_number);
	    }
	    return 1;
	case FL_BackSpace:
	case 0xFE20:		// backtab
	  BACKTAB:
	    if (!backward(p.menu_number)) {
		p.item_number = -1;
		backward(p.menu_number);
	    }
	    return 1;
	case FL_Up:
	    if (p.menubar && p.menu_number == 0);
	    else if (backward(p.menu_number));
	    else if (p.menubar && p.menu_number == 1)
		setitem(0, p.p[0]->selected);
	    return 1;
	case FL_Down:
	    if (p.menu_number || !p.menubar)
		forward(p.menu_number);
	    else if (p.menu_number < p.nummenus - 1)
		forward(p.menu_number + 1);
	    return 1;
	case FL_Right:
	    if (p.menubar
		&& (p.menu_number <= 0 || p.menu_number == 1
		    && p.nummenus == 2))
		forward(0);
	    else if (p.menu_number < p.nummenus - 1)
		forward(p.menu_number + 1);
	    return 1;
	case FL_Left:
	    if (p.menubar && p.menu_number <= 1)
		backward(0);
	    else if (p.menu_number > 0)
		setitem(p.menu_number - 1, p.p[p.menu_number - 1]->selected);
	    return 1;
	case FL_Enter:
	    p.state = DONE_STATE;
	    return 1;
	case FL_Escape:
	    setitem(0, -1, 0);
	    p.state = DONE_STATE;
	    return 1;
	}
	break;
    case FL_SHORTCUT:{
	    for (int menu = p.nummenus; menu--;) {
		menuwindow & mw = *(p.p[menu]);
		int item;
		const Fl_Menu_Item *m = mw.menu->find_shortcut(&item);
		if (m) {
		    setitem(m, menu, item);
		    if (!m->submenu())
			p.state = DONE_STATE;
		    return 1;
		}
	    }
	}
	break;
    case FL_PUSH:
	//case FL_MOVE:
    case FL_DRAG:{
	    int mx = Fl::event_x() + x();
	    int my = Fl::event_y() + y();
	    int item = 0;
	    int menu;
	    for (menu = p.nummenus - 1;; menu--) {
		item = p.p[menu]->find_selected(mx, my);
		if (item >= 0)
		    break;
		if (menu <= 0)
		    break;
	    }
	    setitem(menu, item);
	    if (e == FL_PUSH) {
		if (p.current_item && p.current_item->submenu()	// this is a menu title
		    && item != p.p[menu]->selected	// and it is not already on
		    && !p.current_item->callback_)	// and it does not have a callback
		    p.state = MENU_PUSH_STATE;
		else
		    p.state = PUSH_STATE;
	    }
	}
	return 1;
    case FL_RELEASE:
	// do nothing if they try to pick inactive items
	if (p.current_item && !p.current_item->activevisible())
	    return 1;
	// Mouse must either be held down/dragged some, or this must be
	// the second click (not the one that popped up the menu):
	if (!Fl::event_is_click() || p.state == PUSH_STATE || p.menubar && p.current_item && !p.current_item->submenu()	// button
	    ) {
#if 0				// makes the check/radio items leave the menu up
	    const Fl_Menu_Item *m = p.current_item;
	    if (m && button && (m->flags & (FL_MENU_TOGGLE | FL_MENU_RADIO))) {
		((Fl_Menu_ *) button)->picked(m);
		p.p[p.menu_number]->redraw();
	    } else
#endif
		p.state = DONE_STATE;
	}
	return 1;
    }
    return Fl_Window::handle(e);
}

const Fl_Menu_Item *
Fl_Menu_Item::pulldown(int X, int Y, int W, int H,
		       const Fl_Menu_Item * initial_item,
		       const Fl_Menu_ * pbutton,
		       const Fl_Menu_Item * t, int menubar) const
{
    Fl_Group::current(0);	// fix possible user error...

    button = pbutton;

    if (pbutton) {
	for (Fl_Window * w = pbutton->window(); w; w = w->window()) {
	    X += w->x();
	    Y += w->y();
	}
    } else {
	X += Fl::event_x_root() - Fl::event_x();
	Y += Fl::event_y_root() - Fl::event_y();
    }

    menuwindow mw(this, X, Y, W, H, initial_item, t, menubar);
    Fl::grab(mw);
    menustate p;
    ::p = &p;
    p.p[0] = &mw;
    p.nummenus = 1;
    p.menubar = menubar;
    p.state = INITIAL_STATE;

    menuwindow *fakemenu = 0;	// kludge for buttons in menubar

    // preselected item, pop up submenus if necessary:
    if (initial_item && mw.selected >= 0) {
	setitem(0, mw.selected);
	goto STARTUP;
    }

    p.current_item = 0;
    p.menu_number = 0;
    p.item_number = -1;
    if (menubar)
	mw.handle(FL_DRAG);	// find the initial menu
    initial_item = p.current_item;
    if (initial_item)
	goto STARTUP;

    // the main loop, runs until p.state goes to DONE_STATE:
    for (;;) {

	// make sure all the menus are shown:
	{
	    for (int k = menubar; k < p.nummenus; k++)
		if (!p.p[k]->shown()) {
		    if (p.p[k]->title)
			p.p[k]->title->show();
		    p.p[k]->show();
		}
	}

	// get events:
	{
	    const Fl_Menu_Item *oldi = p.current_item;
	    Fl::wait();
	    if (p.state == DONE_STATE)
		break;		// done.
	    if (p.current_item == oldi)
		continue;
	}
	// only do rest if item changes:

	delete fakemenu;
	fakemenu = 0;		// turn off "menubar button"

	if (!p.current_item) {	// pointing at nothing
	    // turn off selection in deepest menu, but don't erase other menus:
	    p.p[p.nummenus - 1]->set_selected(-1);
	    continue;
	}

	delete fakemenu;
	fakemenu = 0;
	initial_item = 0;	// stop the startup code
	p.p[p.menu_number]->autoscroll(p.item_number);

      STARTUP:
	menuwindow & cw = *p.p[p.menu_number];
	const Fl_Menu_Item *m = p.current_item;
	if (!m->activevisible()) {	// pointing at inactive item
	    cw.set_selected(-1);
	    initial_item = 0;	// turn off startup code
	    continue;
	}
	cw.set_selected(p.item_number);

	if (m == initial_item)
	    initial_item = 0;	// stop the startup code if item found
	if (m->submenu()) {
	    const Fl_Menu_Item *title = m;
	    const Fl_Menu_Item *menutable;
	    if (m->flags & FL_SUBMENU)
		menutable = m + 1;
	    else
		menutable = (Fl_Menu_Item *) (m)->user_data_;
	    // figure out where new menu goes:
	    int nX, nY;
	    if (!p.menu_number && p.menubar) {	// menu off a menubar:
		nX = cw.x() + cw.titlex(p.item_number);
		nY = cw.y() + cw.h();
		initial_item = 0;
	    } else {
		nX = cw.x() + cw.w();
		nY = cw.y() + 1 + p.item_number * cw.itemheight;
		title = 0;
	    }
	    if (initial_item) {	// bring up submenu containing initial item:
		menuwindow *n =
		    new menuwindow(menutable, X, Y, W, H, initial_item,
				   title);
		p.p[p.nummenus++] = n;
		// move all earlier menus to line up with this new one:
		if (n->selected >= 0) {
		    int dy = n->y() - nY;
		    int dx = n->x() - nX;
		    for (int menu = 0; menu <= p.menu_number; menu++) {
			menuwindow *t = p.p[menu];
			int nx = t->x() + dx;
			if (nx < 0) {
			    nx = 0;
			    dx = -t->x();
			}
			int ny = t->y() + dy + 1;
			if (ny < 0) {
			    ny = 0;
			    dy = -t->y() - 1;
			}
			t->position(nx, ny);
		    }
		    setitem(p.nummenus - 1, n->selected);
		    goto STARTUP;
		}
	    } else if (p.nummenus > p.menu_number + 1 &&
		       p.p[p.menu_number + 1]->menu == menutable) {
		// the menu is already up:
		while (p.nummenus > p.menu_number + 2)
		    delete p.p[--p.nummenus];
		p.p[p.nummenus - 1]->set_selected(-1);
	    } else {
		// delete all the old menus and create new one:
		while (p.nummenus > p.menu_number + 1)
		    delete p.p[--p.nummenus];
		p.p[p.nummenus++] = new menuwindow(menutable, nX, nY,
						   title ? 1 : 0, 0, 0, title,
						   0, menubar);
	    }
	} else {		// !m->submenu():
	    while (p.nummenus > p.menu_number + 1)
		delete p.p[--p.nummenus];
	    if (!p.menu_number && p.menubar) {
		// kludge so "menubar buttons" turn "on" by using menu title:
		fakemenu = new menuwindow(0,
					  cw.x() + cw.titlex(p.item_number),
					  cw.y() + cw.h(), 0, 0, 0, m, 0, 1);
		fakemenu->title->show();
	    }
	}
    }
    const Fl_Menu_Item *m = p.current_item;
    delete fakemenu;
    while (p.nummenus > 1)
	delete p.p[--p.nummenus];
    mw.hide();
    Fl::release();
    return m;
}

const Fl_Menu_Item *
Fl_Menu_Item::popup(int X, int Y,
		    const char *title,
		    const Fl_Menu_Item * picked,
		    const Fl_Menu_ * button) const
{
    static Fl_Menu_Item dummy;	// static so it is all zeros
    dummy.text = title;
    return pulldown(X, Y, 0, 0, picked, button, title ? &dummy : 0);
}

const Fl_Menu_Item *
Fl_Menu_Item::test_shortcut() const
{
    const Fl_Menu_Item *m = this;
    const Fl_Menu_Item *ret = 0;
    if (m)
	for (; m->text; m = m->next()) {
	    if (m->activevisible()) {
		// return immediately any match of an item in top level menu:
		if (Fl::test_shortcut(m->shortcut_))
		    return m;
		// if (Fl_Widget::test_shortcut(m->text)) return m;
		// only return matches from lower menu if nothing found in top menu:
		if (!ret && m->submenu()) {
		    const Fl_Menu_Item *s =
			(m->flags & FL_SUBMENU) ? m +
			1 : (const Fl_Menu_Item *) m->user_data_;
		    ret = s->test_shortcut();
		}
	    }
	}
    return ret;
}

//
// End of "$Id: Fl_Menu.cxx 1425 2006-10-03 11:27:15Z dslinux_amadeus $".
//
