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
   @file pelican_memory.c
   Pelican MAC, MAC a block of memory, by Tom St Denis 
*/

#ifdef PELICAN

/**
  Pelican block of memory
  @param key      The key for the MAC
  @param keylen   The length of the key (octets)
  @param in       The input to MAC
  @param inlen    The length of the input (octets)
  @param out      [out] The output TAG 
  @return CRYPT_OK on success
*/
int pelican_memory(const unsigned char *key, unsigned long keylen,
                   const unsigned char *in,  unsigned long inlen,
                         unsigned char *out)
{
   pelican_state *pel;
   int err;

   pel = XMALLOC(sizeof(*pel));
   if (pel == NULL) { 
      return CRYPT_MEM;
   }

   if ((err = pelican_init(pel, key, keylen)) != CRYPT_OK) {
      XFREE(pel);
      return err;
   }
   if ((err = pelican_process(pel, in ,inlen)) != CRYPT_OK) {
      XFREE(pel);
      return err;
   }
   err = pelican_done(pel, out);
   XFREE(pel); 
   return err;
}


#endif

/* $Source$ */
/* $Revision: 1054 $ */
/* $Date: 2006-05-23 01:58:10 +0200 (Di 23 de Mai de 2006) $ */
