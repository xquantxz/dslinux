//
// "$Id: Fl_Check_Button.cxx 1425 2006-10-03 11:27:15Z dslinux_amadeus $"
//
// Check button widget for the Fast Light Tool Kit (FLTK).
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

#include <FL/Fl.H>
#include <FL/Fl_Check_Button.H>

// A subclass of Fl_Button that always draws as a diamond box.  This
// diamond is smaller than the widget size and can be surchecked by
// another box type, for compatability with Forms.

Fl_Check_Button::Fl_Check_Button(int x, int y, int w, int h, const char *l)
: Fl_Light_Button(x, y, w, h, l) {
  box(FL_NO_BOX);
  down_box(FL_DIAMOND_DOWN_BOX);
  selection_color(FL_RED);
}
