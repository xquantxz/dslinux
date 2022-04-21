/*                                                                       
 * Copyright (c) 2003 Century Software, Inc.   All Rights Reserved.     
 *                                                                       
 * This file is part of the PIXIL Operating Environment                 
 *                                                                       
 * The use, copying and distribution of this file is governed by one    
 * of two licenses, the PIXIL Commercial License, or the GNU General    
 * Public License, version 2.                                           
 *                                                                       
 * Licensees holding a valid PIXIL Commercial License may use this file 
 * in accordance with the PIXIL Commercial License Agreement provided   
 * with the Software. Others are governed under the terms of the GNU   
 * General Public License version 2.                                    
 *                                                                       
 * This file may be distributed and/or modified under the terms of the  
 * GNU General Public License version 2 as published by the Free        
 * Software Foundation and appearing in the file LICENSE.GPL included   
 * in the packaging of this file.                                      
 *                                                                       
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING  
 * THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A            
 * PARTICULAR PURPOSE.                                                  
 *                                                                       
 * RESTRICTED RIGHTS LEGEND                                             
 *                                                                     
 * Use, duplication, or disclosure by the government is subject to      
 * restriction as set forth in paragraph (b)(3)(b) of the Rights in     
 * Technical Data and Computer Software clause in DAR 7-104.9(a).       
 *                                                                      
 * See http://www.pixil.org/gpl/ for GPL licensing       
 * information.                                                         
 *                                                                      
 * See http://www.pixil.org/license.html or              
 * email cetsales@centurysoftware.com for information about the PIXIL   
 * Commercial License Agreement, or if any conditions of this licensing 
 * are not clear to you.                                                
 */


#include <nxapp.h>
#include <FL/Enumerations.H>
#include <nxradioroundbutton.h>

void
round_draw(int x, int y, int w, int h, Fl_Color c)
{

    Fl_Color bf = NxApp::Instance()->getGlobalColor(APP_SEL);
    fl_color(bf);
    fl_draw_box(FL_ROUNDED_BOX, x + 3, y + 4, w - 10, h - 10, bf);

}

NxRadioRoundButton::NxRadioRoundButton(int x, int y, int w, int h, char *l):
Fl_Radio_Round_Button(x, y, w, h, l)
{

    // Provide the "look-and-feel"
    color(NxApp::Instance()->getGlobalColor(APP_BG));
    selection_color(NxApp::Instance()->getGlobalColor(RADIO_FILL));
    labelcolor(NxApp::Instance()->getGlobalColor(APP_FG));
    NxApp::Instance()->def_font((Fl_Widget *) this);
#ifdef PDA
    Fl::set_boxtype(FL_WHITE_BOX, round_draw, 1, 1, 2, 2);
    box(FL_WHITE_BOX);
#else
    box(FL_NO_BOX);
#endif
    align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
    value(0);
}				// end of NxRadioRoundButton::NxRadioRoundButton()

void
NxRadioRoundButton::draw()
{

    if (box())
	draw_box(this == Fl::pushed()? down(box()) : box(), color());

    int d = h() / 6;
    int W = w() < h()? w() : h();

    if (value())
	fl_draw_box(FL_ROUNDED_BOX, x() + d + 2, y() + d + 3, W - 4 * d - 2,
		    W - 4 * d - 2, selection_color());

#ifdef PDA
    labelcolor(labelcolor());
#endif

    draw_label(x() + W - d, y(), w() - W + d, h());
}
