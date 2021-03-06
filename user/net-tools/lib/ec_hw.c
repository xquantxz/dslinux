/*
 * lib/ec_hw.c  This file contains an implementation of the Econet
 *              hardware support functions.
 *
 * Version:     $Id: ec_hw.c 1363 2006-08-31 09:32:46Z dslinux_amadeus $
 *
 * Author:      Philip Blundell <philb@gnu.org>
 *
 *              This program is free software; you can redistribute it
 *              and/or  modify it under  the terms of  the GNU General
 *              Public  License as  published  by  the  Free  Software
 *              Foundation;  either  version 2 of the License, or  (at
 *              your option) any later version.
 */
#include "config.h"

#if HAVE_HWEC

#include <net/if_arp.h>
#include "net-support.h"

struct hwtype ec_hwtype =
{
    "ec", NULL /* "Econet" */, ARPHRD_ECONET, 0,
    NULL, NULL, NULL
};

#endif
