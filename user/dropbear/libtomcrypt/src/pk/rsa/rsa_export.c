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
  @file rsa_export.c
  Export RSA PKCS keys, Tom St Denis
*/  

#ifdef MRSA

/**
    This will export either an RSAPublicKey or RSAPrivateKey [defined in PKCS #1 v2.1] 
    @param out       [out] Destination of the packet
    @param outlen    [in/out] The max size and resulting size of the packet
    @param type      The type of exported key (PK_PRIVATE or PK_PUBLIC)
    @param key       The RSA key to export
    @return CRYPT_OK if successful
*/    
int rsa_export(unsigned char *out, unsigned long *outlen, int type, rsa_key *key)
{
   int           err;
   unsigned long zero=0;

   LTC_ARGCHK(out    != NULL);
   LTC_ARGCHK(outlen != NULL);
   LTC_ARGCHK(key    != NULL);

   /* type valid? */
   if (!(key->type == PK_PRIVATE) && (type == PK_PRIVATE)) {
      return CRYPT_PK_INVALID_TYPE;
   }

   if (type == PK_PRIVATE) {
      /* private key */
      /* output is 
            Version, n, e, d, p, q, d mod (p-1), d mod (q - 1), 1/q mod p
       */
      if ((err = der_encode_sequence_multi(out, outlen, 
                          LTC_ASN1_SHORT_INTEGER, 1UL, &zero, 
                          LTC_ASN1_INTEGER, 1UL, &key->N, 
                          LTC_ASN1_INTEGER, 1UL, &key->e,
                          LTC_ASN1_INTEGER, 1UL, &key->d, 
                          LTC_ASN1_INTEGER, 1UL, &key->p, 
                          LTC_ASN1_INTEGER, 1UL, &key->q, 
                          LTC_ASN1_INTEGER, 1UL, &key->dP,
                          LTC_ASN1_INTEGER, 1UL, &key->dQ, 
                          LTC_ASN1_INTEGER, 1UL, &key->qP, 
                          LTC_ASN1_EOL,     0UL, NULL)) != CRYPT_OK) {
         return err;
      }
 
      /* clear zero and return */
      return CRYPT_OK;
   } else {
      /* public key */
      return der_encode_sequence_multi(out, outlen, 
                                 LTC_ASN1_INTEGER, 1UL, &key->N, 
                                 LTC_ASN1_INTEGER, 1UL, &key->e, 
                                 LTC_ASN1_EOL,     0UL, NULL);
   }
}

#endif /* MRSA */

/* $Source$ */
/* $Revision: 1054 $ */
/* $Date: 2006-05-23 01:58:10 +0200 (Di 23 de Mai de 2006) $ */
