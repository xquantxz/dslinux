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
#include "tomcrypt.h"

/**
   @file dsa_free.c
   DSA implementation, free a DSA key, Tom St Denis
*/

#ifdef MDSA

/**
   Free a DSA key
   @param key   The key to free from memory
*/
void dsa_free(dsa_key *key)
{
   LTC_ARGCHK(key != NULL);
   mp_clear_multi(&key->g, &key->q, &key->p, &key->x, &key->y, NULL);
}

#endif

/* $Source$ */
/* $Revision: 1054 $ */
/* $Date: 2006-05-23 01:58:10 +0200 (Di 23 de Mai de 2006) $ */
