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
   @file cbc_done.c
   CBC implementation, finish chain, Tom St Denis
*/

#ifdef CBC

/** Terminate the chain
  @param cbc    The CBC chain to terminate
  @return CRYPT_OK on success
*/
int cbc_done(symmetric_CBC *cbc)
{
   int err;
   LTC_ARGCHK(cbc != NULL);

   if ((err = cipher_is_valid(cbc->cipher)) != CRYPT_OK) {
      return err;
   }
   cipher_descriptor[cbc->cipher].done(&cbc->key);
   return CRYPT_OK;
}

   

#endif

/* $Source$ */
/* $Revision: 1054 $ */
/* $Date: 2006-05-23 01:58:10 +0200 (Di 23 de Mai de 2006) $ */
