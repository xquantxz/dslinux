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
  @file is_prime.c
  Determines if integer is prime for LTC, Tom St Denis
*/  

#ifdef MPI

/* figures out if a number is prime (MR test) */
int is_prime(mp_int *N, int *result)
{
   int err;
   LTC_ARGCHK(N != NULL);
   LTC_ARGCHK(result != NULL);
   if ((err = mp_prime_is_prime(N, mp_prime_rabin_miller_trials(mp_count_bits(N)), result)) != MP_OKAY) {
      return mpi_to_ltc_error(err);
   }
   return CRYPT_OK;
}

#endif

/* $Source$ */
/* $Revision: 1054 $ */
/* $Date: 2006-05-23 01:58:10 +0200 (Di 23 de Mai de 2006) $ */
