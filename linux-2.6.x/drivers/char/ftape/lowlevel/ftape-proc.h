#ifndef _FTAPE_PROC_H
#define _FTAPE_PROC_H

/*
 *      Copyright (C) 1997 Claus-Justus Heine

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 *
 * $Source$
 * $Revision: 573 $
 * $Date: 2006-02-20 18:09:11 +0100 (Mo 20 de Feb de 2006) $
 *
 *      This file contains definitions for the procfs interface of the
 *      QIC-40/80/3010/3020 floppy-tape driver "ftape" for Linux.
 */

#include <linux/proc_fs.h>

extern int  ftape_proc_init(void);
extern void ftape_proc_destroy(void);

#endif
