// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id: d_net.h 1425 2006-10-03 11:27:15Z dslinux_amadeus $
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
//	Networking stuff.
//
//-----------------------------------------------------------------------------


#ifndef __D_NET__
#define __D_NET__

#include "d_player.h"


#ifdef __GNUG__
#pragma interface
#endif


//
// Network play related stuff.
// There is a data struct that stores network
//  communication related stuff, and another
//  one that defines the actual packets to
//  be transmitted.
//

#define DOOMCOM_ID		0x12345678l

// Max computers/players in a game.
#define MAXNETNODES		8


// Networking and tick handling related.
#define BACKUPTICS		12

typedef enum
{
    CMD_SEND	= 1,
    CMD_GET	= 2

} command_t;


//
// Network packet data.
//
typedef struct
{
    // High bit is retransmit request.
    unsigned		checksum;
    // Only valid if NCMD_RETRANSMIT.
    byte		retransmitfrom;
    
    byte		starttic;
    byte		player;
    byte		numtics;
    ticcmd_t		cmds[BACKUPTICS];

} doomdata_t;




typedef struct
{
    // Supposed to be DOOMCOM_ID?
    long		id;
    
    // DOOM executes an int to execute commands.
    short		intnum;		
    // Communication between DOOM and the driver.
    // Is CMD_SEND or CMD_GET.
    short		command;
    // Is dest for send, set by get (-1 = no packet).
    short		remotenode;
    
    // Number of bytes in doomdata to be sent
    short		datalength;

    // Info common to all nodes.
    // Console is allways node 0.
    short		numnodes;
    // Flag: 1 = no duplication, 2-5 = dup for slow nets.
    short		ticdup;
    // Flag: 1 = send a backup tic in every packet.
    short		extratics;
    // Flag: 1 = deathmatch.
    short		deathmatch;
    // Flag: -1 = new game, 0-5 = load savegame
    short		savegame;
    short		episode;	// 1-3
    short		map;		// 1-9
    short		skill;		// 1-5

    // Info specific to this node.
    short		consoleplayer;
    short		numplayers;
    
    // These are related to the 3-display mode,
    //  in which two drones looking left and right
    //  were used to render two additional views
    //  on two additional computers.
    // Probably not operational anymore.
    // 1 = left, 0 = center, -1 = right
    short		angleoffset;
    // 1 = drone
    short		drone;		

    // The packet data to be sent.
    doomdata_t		data;
    
} doomcom_t;



// Create any new ticcmds and broadcast to other players.
void NetUpdate (void);

// Broadcasts special packets to other players
//  to notify of game exit
void D_QuitNetGame (void);

//? how many ticks to run?
void TryRunTics (void);


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

