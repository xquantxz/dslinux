//
// "$Id: fl_font_win32.cxx 1884 2007-09-13 23:19:15Z dslinux_amadeus $"
//
// WIN32 font selection routines for the Fast Light Tool Kit (FLTK).
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

#include <config.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/win32.H>
#include "Fl_Font.H"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

Fl_FontSize::Fl_FontSize(const char* name, int size) {
  int weight = FW_NORMAL;
  int italic = 0;
  switch (*name++) {
  case 'I': italic = 1; break;
  case 'P': italic = 1;
  case 'B': weight = FW_BOLD; break;
  case ' ': break;
  default: name--;
  }
  fid = CreateFont(
    -size, // negative makes it use "char size"
    0,	            // logical average character width 
    0,	            // angle of escapement 
    0,	            // base-line orientation angle 
    weight,
    italic,
    FALSE,	        // underline attribute flag 
    FALSE,	        // strikeout attribute flag 
    DEFAULT_CHARSET,    // character set identifier 
    OUT_DEFAULT_PRECIS,	// output precision 
    CLIP_DEFAULT_PRECIS,// clipping precision 
    DEFAULT_QUALITY,	// output quality 
    DEFAULT_PITCH,	// pitch and family 
    name	        // pointer to typeface name string 
    );
  if (!fl_gc) fl_GetDC(0);
  SelectObject(fl_gc, fid);
  GetTextMetrics(fl_gc, &metr);
//  BOOL ret = GetCharWidthFloat(fl_gc, metr.tmFirstChar, metr.tmLastChar, font->width+metr.tmFirstChar);
// ...would be the right call, but is not implemented into Window95! (WinNT?)
  GetCharWidth(fl_gc, 0, 255, width);
#if HAVE_GL
  listbase = 0;
#endif
  minsize = maxsize = size;
}

Fl_FontSize* fl_fontsize;

Fl_FontSize::~Fl_FontSize() {
#if HAVE_GL
// Delete list created by gl_draw().  This is not done by this code
// as it will link in GL unnecessarily.  There should be some kind
// of "free" routine pointer, or a subclass?
// if (listbase) {
//  int base = font->min_char_or_byte2;
//  int size = font->max_char_or_byte2-base+1;
//  int base = 0; int size = 256;
//  glDeleteLists(listbase+base,size);
// }
#endif
  if (this == fl_fontsize) fl_fontsize = 0;
  DeleteObject(fid);
}

////////////////////////////////////////////////////////////////

// WARNING: if you add to this table, you must redefine FL_FREE_FONT
// in Enumerations.H & recompile!!
static Fl_Fontdesc built_in_table[] = {
{" Arial"},
{"BArial"},
{"IArial"},
{"PArial"},
{" Courier New"},
{"BCourier New"},
{"ICourier New"},
{"PCourier New"},
{" Times New Roman"},
{"BTimes New Roman"},
{"ITimes New Roman"},
{"PTimes New Roman"},
{" Symbol"},
{" Terminal"},
{"BTerminal"},
{" Wingdings"},
};

Fl_Fontdesc* fl_fonts = built_in_table;

static Fl_FontSize* find(int fnum, int size) {
  Fl_Fontdesc* s = fl_fonts+fnum;
  if (!s->name) s = fl_fonts; // use 0 if fnum undefined
  Fl_FontSize* f;
  for (f = s->first; f; f = f->next)
    if (f->minsize <= size && f->maxsize >= size) return f;
  f = new Fl_FontSize(s->name, size);
  f->next = s->first;
  s->first = f;
  return f;
}

////////////////////////////////////////////////////////////////
// Public interface:

int fl_font_;
int fl_size_;
//static HDC font_gc;

void fl_font(int fnum, int size) {
  if (fnum == fl_font_ && size == fl_size_) return;
  fl_font_ = fnum; fl_size_ = size;
  fl_fontsize = find(fnum, size);
}

int fl_height() {
  return (fl_fontsize->metr.tmAscent + fl_fontsize->metr.tmDescent);
}

int fl_descent() {
  return fl_fontsize->metr.tmDescent;
}

float fl_width(const char* c) {
  float w = 0.0;
  while (*c) w += fl_fontsize->width[uchar(*c++)];
  return w;
}

float fl_width(const char* c, int n) {
  float w = 0.0;
  while (n--) w += fl_fontsize->width[uchar(*c++)];
  return w;
}

float fl_width(uchar c) {
  return fl_fontsize->width[c];
}

void fl_draw(const char* str, int n, int x, int y) {
  SetTextColor(fl_gc, fl_RGB());
  SelectObject(fl_gc, fl_fontsize->fid);
  TextOut(fl_gc, x, y, str, n);
}

void fl_draw(const char* str, int x, int y) {
  fl_draw(str, strlen(str), x, y);
}

//
// End of "$Id: fl_font_win32.cxx 1884 2007-09-13 23:19:15Z dslinux_amadeus $".
//
