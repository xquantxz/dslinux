//
// "$Id: forms_bitmap.cxx 1425 2006-10-03 11:27:15Z dslinux_amadeus $"
//
// Forms compatible bitmap function for the Fast Light Tool Kit (FLTK).
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

#include <FL/forms.H>

Fl_FormsBitmap::Fl_FormsBitmap(
  Fl_Boxtype t, int x, int y, int w, int h, const char* l)
: Fl_Widget(x, y, w, h, l) {
  box(t);
  b = 0;
  color(FL_BLACK);
  align(FL_ALIGN_BOTTOM);
}

void Fl_FormsBitmap::set(int W, int H, const uchar *bits) {
  delete b;
  bitmap(new Fl_Bitmap(bits, W, H));
}

void Fl_FormsBitmap::draw() {
  draw_box(box(), selection_color());
  if (b) {fl_color(color()); b->draw(x(), y(), w(), h());}
  draw_label();
}

//
// End of "$Id: forms_bitmap.cxx 1425 2006-10-03 11:27:15Z dslinux_amadeus $".
//
