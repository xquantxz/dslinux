// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id: i_system.c 1425 2006-10-03 11:27:15Z dslinux_amadeus $
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
// $Log$
// Revision 1.1  2006/10/03 11:26:17  dslinux_amadeus
// adding pristine copy of pixil to HEAD so I can branch from it
//
// Revision 1.2  2003/09/08 22:34:28  jasonk
// Updated files because this fucker won't build for no fucking good reason.
//
// Revision 1.1.1.1  2003/09/04 21:08:12  jasonk
// Initial import
//
// Revision 1.1  2000/12/08 21:07:53  jeffw
// nxdoom initial entry -- No nxdoom/Makefile so it won't build automatically
//
//
// DESCRIPTION:
//
//-----------------------------------------------------------------------------

static const char
rcsid[] = "$Id: i_system.c 1425 2006-10-03 11:27:15Z dslinux_amadeus $";


#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/time.h>
#include <nano-X.h>
#include "doomdef.h"
#include "m_misc.h"
#include "i_video.h"
#include "i_sound.h"
#include "d_net.h"
#include "g_game.h"

#ifdef __GNUG__
#pragma implementation "i_system.h"
#endif
#include "i_system.h"


int	mb_used = 6;


int I_strncasecmp(char *str1, char *str2, int len)
{
	char c1, c2;

	while ( *str1 && *str2 && len-- ) {
		c1 = *str1++;
		c2 = *str2++;
		if ( toupper(c1) != toupper(c2) )
			return(1);
	}
	return(0);
}

void
I_Tactile
( int	on,
  int	off,
  int	total )
{
  // UNUSED.
  on = off = total = 0;
}

ticcmd_t	emptycmd;
ticcmd_t*	I_BaseTiccmd(void)
{
    return &emptycmd;
}


int  I_GetHeapSize (void)
{
    return mb_used*1024*1024;
}

byte* I_ZoneBase (int*	size)
{
    *size = mb_used*1024*1024;
    return (byte *) malloc (*size);
}

static unsigned long
GetTicks(void)
{
	struct timeval now;
	unsigned long ticks;
	static struct timeval start;

	if (start.tv_sec == 0) {
		/* Set first ticks value */
		gettimeofday(&start, NULL);
	}

	gettimeofday(&now, NULL);
	ticks=(now.tv_sec-start.tv_sec)*1000+(now.tv_usec-start.tv_usec)/1000;
	return(ticks);
}

//
// I_GetTime
// returns time in 1/35 second tics
//
int  I_GetTime (void)
{
    return (GetTicks()*TICRATE)/1000;
}


//
// I_Init
//
void I_Init (void)
{
    if (GrOpen() < 0)
        I_Error("Can't open graphics\n");

#if defined(USE_DSP) || defined(USE_ESOUND)
    I_InitSound();
#endif

    //  I_InitGraphics();
}

//
// I_Quit
//
void I_Quit (void)
{
    D_QuitNetGame ();

#if defined(USE_DSP) || defined(USE_ESOUND)
    I_ShutdownSound();
    I_ShutdownMusic();
#endif

    M_SaveDefaults ();
    I_ShutdownGraphics();
    exit(0);
}

void I_WaitVBL(int count)
{
    //SDL_Delay((count*1000)/70);
}

void I_BeginRead(void)
{
}

void I_EndRead(void)
{
}

byte*	I_AllocLow(int length)
{
    byte*	mem;
        
    mem = (byte *)malloc (length);
    memset (mem,0,length);
    return mem;
}


//
// I_Error
//
extern boolean demorecording;

void I_Error (char *error, ...)
{
    va_list	argptr;

    // Message first.
    va_start (argptr,error);
    fprintf (stderr, "Error: ");
    vfprintf (stderr,error,argptr);
    fprintf (stderr, "\n");
    va_end (argptr);

    fflush( stderr );

    // Shutdown. Here might be other errors.
    if (demorecording)
	G_CheckDemoStatus();

    D_QuitNetGame ();
    I_ShutdownGraphics();
    
    exit(-1);
}
