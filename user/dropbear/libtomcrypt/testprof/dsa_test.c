#include <tomcrypt_test.h>

#ifdef MDSA

int dsa_test(void)
{
   unsigned char msg[16], out[1024], out2[1024];
   unsigned long x;
   int stat1, stat2;
   dsa_key key, key2;

   /* make a random key */
   DO(dsa_make_key(&yarrow_prng, find_prng("yarrow"), 20, 128, &key));

   /* verify it */
   DO(dsa_verify_key(&key, &stat1));
   if (stat1 == 0) { fprintf(stderr, "dsa_verify_key "); return 1; }

   /* sign the message */
   x = sizeof(out);
   DO(dsa_sign_hash(msg, sizeof(msg), out, &x, &yarrow_prng, find_prng("yarrow"), &key));

   /* verify it once */
   DO(dsa_verify_hash(out, x, msg, sizeof(msg), &stat1, &key));

   /* Modify and verify again */
   msg[0] ^= 1;
   DO(dsa_verify_hash(out, x, msg, sizeof(msg), &stat2, &key));
   msg[0] ^= 1;
   if (!(stat1 == 1 && stat2 == 0)) { fprintf(stderr, "dsa_verify %d %d", stat1, stat2); return 1; }

   /* test exporting it */
   x = sizeof(out2);
   DO(dsa_export(out2, &x, PK_PRIVATE, &key));
   DO(dsa_import(out2, x, &key2));

   /* verify a signature with it */
   DO(dsa_verify_hash(out, x, msg, sizeof(msg), &stat1, &key2));
   if (stat1 == 0) { fprintf(stderr, "dsa_verify (import private) %d ", stat1); return 1; }
   dsa_free(&key2);

   /* export as public now */
   x = sizeof(out2);
   DO(dsa_export(out2, &x, PK_PUBLIC, &key));

   DO(dsa_import(out2, x, &key2));
   /* verify a signature with it */
   DO(dsa_verify_hash(out, x, msg, sizeof(msg), &stat1, &key2));
   if (stat1 == 0) { fprintf(stderr, "dsa_verify (import public) %d ", stat1); return 1; }
   dsa_free(&key2);
   dsa_free(&key);

   return 0;
}

#else

int dsa_test(void)
{
  fprintf(stderr, "NOP");
  return 0;
}

#endif

/* $Source$ */
/* $Revision: 1054 $ */
/* $Date: 2006-05-23 01:58:10 +0200 (Di 23 de Mai de 2006) $ */
