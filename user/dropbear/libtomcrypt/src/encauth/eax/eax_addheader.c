/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 *
 * Tom St Denis, tomstdenis@gmail.com, http://libtomcrypt.org
 */
/** 
    @file eax_addheader.c
    EAX implementation, add meta-data, by Tom St Denis 
*/
#include "tomcrypt.h"

#ifdef EAX_MODE

/** 
    add header (metadata) to the stream 
    @param eax    The current EAX state
    @param header The header (meta-data) data you wish to add to the state
    @param length The length of the header data
    @return CRYPT_OK if successful
*/
int eax_addheader(eax_state *eax, const unsigned char *header, 
                  unsigned long length)
{
   LTC_ARGCHK(eax    != NULL);
   LTC_ARGCHK(header != NULL);
   return omac_process(&eax->headeromac, header, length);
}

#endif

/* $Source$ */
/* $Revision: 1054 $ */
/* $Date: 2006-05-23 01:58:10 +0200 (Di 23 de Mai de 2006) $ */
