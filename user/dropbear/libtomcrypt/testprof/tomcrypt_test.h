
#ifndef __TEST_H_
#define __TEST_H_

#include <tomcrypt.h>

/* enable stack testing */
// #define STACK_TEST

/* stack testing, define this if stack usage goes downwards [e.g. x86] */
#define STACK_DOWN

typedef struct {
    char *name, *prov, *req;
    int  (*entry)(void);
} test_entry;

extern prng_state yarrow_prng;

void run_cmd(int res, int line, char *file, char *cmd);
#define DO(x) { run_cmd((x), __LINE__, __FILE__, #x); }

/* TESTS */
int cipher_hash_test(void);
int modes_test(void);
int mac_test(void);
int pkcs_1_test(void);
int store_test(void);
int rsa_test(void);
int ecc_tests(void);
int dsa_test(void);
int dh_tests(void);
int der_tests(void);

/* timing */
#define KTIMES  25
#define TIMES   100000

extern struct list {
    int id;
    unsigned long spd1, spd2, avg;
} results[];

extern int no_results;

int sorter(const void *a, const void *b);
void tally_results(int type);
ulong64 rdtsc (void);

void t_start(void);
ulong64 t_read(void);
void init_timer(void);

/* register default algs */
void reg_algs(void);
int time_keysched(void);
int time_cipher(void);
int time_cipher2(void);
int time_cipher3(void);
int time_hash(void);
void time_mult(void);
void time_sqr(void);
void time_prng(void);
void time_rsa(void);
void time_ecc(void);
void time_dh(void);
void time_macs_(unsigned long MAC_SIZE);
void time_macs(void);
void time_encmacs(void);



#endif

/* $Source$ */
/* $Revision: 1054 $ */
/* $Date: 2006-05-23 01:58:10 +0200 (Di 23 de Mai de 2006) $ */
