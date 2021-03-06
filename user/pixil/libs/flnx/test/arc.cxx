//
// "$Id: arc.cxx 1884 2007-09-13 23:19:15Z dslinux_amadeus $"
//
// Arc drawing test program for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Hor_Value_Slider.H>
#include <FL/fl_draw.H>

float args[6] = {140, 140, 50, 0, 360, 0};
const char* name[6] = {"X", "Y", "R", "start", "end", "rotate"};

class Drawing : public Fl_Widget {
  void draw() {
    fl_clip(x(),y(),w(),h());
    fl_color(FL_DARK3);
    fl_rectf(x(),y(),w(),h());
    fl_push_matrix();
    if (args[5]) {
      fl_translate(x()+w()/2.0, y()+h()/2.0);
      fl_rotate(args[5]);
      fl_translate(-(x()+w()/2.0), -(y()+h()/2.0));
    }
    fl_color(FL_WHITE);
    fl_translate(x(),y());
    fl_begin_complex_polygon();
    fl_arc(args[0],args[1],args[2],args[3],args[4]);
    fl_gap();
    fl_arc(140,140,20,0,-360);
    fl_end_complex_polygon();
    fl_color(FL_RED);
    fl_begin_line();
    fl_arc(args[0],args[1],args[2],args[3],args[4]);
    fl_end_line();
    fl_pop_matrix();
    fl_pop_clip();
  }
public:
  Drawing(int X,int Y,int W,int H) : Fl_Widget(X,Y,W,H) {}
};

Drawing *d;

void slider_cb(Fl_Widget* o, void* v) {
  Fl_Slider* s = (Fl_Slider*)o;
  args[long(v)] = s->value();
  d->redraw();
}

int main(int argc, char** argv) {
  Fl_Double_Window window(300,500);
  Drawing drawing(10,10,280,280);
  d = &drawing;

  int y = 300;
  for (int n = 0; n<6; n++) {
    Fl_Slider* s = new Fl_Hor_Value_Slider(50,y,240,25,name[n]); y += 25;
    if (n<3) {s->minimum(0); s->maximum(300);}
    else if (n==5) {s->minimum(0); s->maximum(360);}
    else {s->minimum(-360); s->maximum(360);}
    s->step(1);
    s->value(args[n]);
    s->align(FL_ALIGN_LEFT);
    s->callback(slider_cb, (void*)n);
  }

  window.end();
  window.show(argc,argv);
  return Fl::run();
}


//
// End of "$Id: arc.cxx 1884 2007-09-13 23:19:15Z dslinux_amadeus $".
//

