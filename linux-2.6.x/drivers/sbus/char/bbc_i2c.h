/* $Id: bbc_i2c.h 573 2006-02-20 17:09:11Z stsp2 $ */
#ifndef _BBC_I2C_H
#define _BBC_I2C_H

#include <asm/ebus.h>

struct bbc_i2c_client;

/* Probing and attachment. */
extern struct linux_ebus_child *bbc_i2c_getdev(int);
extern struct bbc_i2c_client *bbc_i2c_attach(struct linux_ebus_child *);
extern void bbc_i2c_detach(struct bbc_i2c_client *);

/* Register read/write.  NOTE: Blocking! */
extern int bbc_i2c_writeb(struct bbc_i2c_client *, unsigned char val, int off);
extern int bbc_i2c_readb(struct bbc_i2c_client *, unsigned char *byte, int off);
extern int bbc_i2c_write_buf(struct bbc_i2c_client *, char *buf, int len, int off);
extern int bbc_i2c_read_buf(struct bbc_i2c_client *, char *buf, int len, int off);

#endif /* _BBC_I2C_H */
