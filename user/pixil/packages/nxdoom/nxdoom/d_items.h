// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id: d_items.h 1425 2006-10-03 11:27:15Z dslinux_amadeus $
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// DESCRIPTION:
//	Items: key cards, artifacts, weapon, ammunition.
//
//-----------------------------------------------------------------------------


#ifndef __D_ITEMS__
#define __D_ITEMS__

#include "doomdef.h"

#ifdef __GNUG__
#pragma interface
#endif


// Weapon info: sprite frames, ammunition use.
typedef struct
{
    ammotype_t	ammo;
    int		upstate;
    int		downstate;
    int		readystate;
    int		atkstate;
    int		flashstate;

} weaponinfo_t;

extern  weaponinfo_t    weaponinfo[NUMWEAPONS];

#endif
//-----------------------------------------------------------------------------
//
// $Log$
// Revision 1.1  2006/10/03 11:26:15  dslinux_amadeus
// adding pristine copy of pixil to HEAD so I can branch from it
//
// Revision 1.2  2003/09/08 22:34:27  jasonk
// Updated files because this fucker won't build for no fucking good reason.
//
// Revision 1.1.1.1  2003/09/04 21:08:12  jasonk
// Initial import
//
// Revision 1.1  2000/12/08 21:07:51  jeffw
// nxdoom initial entry -- No nxdoom/Makefile so it won't build automatically
//
//
//-----------------------------------------------------------------------------
