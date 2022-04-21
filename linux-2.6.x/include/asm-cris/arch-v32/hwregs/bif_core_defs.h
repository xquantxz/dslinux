#ifndef __bif_core_defs_h
#define __bif_core_defs_h

/*
 * This file is autogenerated from
 *   file:           ../../inst/bif/rtl/bif_core_regs.r
 *     id:           bif_core_regs.r,v 1.17 2005/02/04 13:28:22 np Exp
 *     last modfied: Mon Apr 11 16:06:33 2005
 *
 *   by /n/asic/design/tools/rdesc/src/rdes2c --outfile bif_core_defs.h ../../inst/bif/rtl/bif_core_regs.r
 *      id: $Id: bif_core_defs.h 573 2006-02-20 17:09:11Z stsp2 $
 * Any changes here will be lost.
 *
 * -*- buffer-read-only: t -*-
 */
/* Main access macros */
#ifndef REG_RD
#define REG_RD( scope, inst, reg ) \
  REG_READ( reg_##scope##_##reg, \
            (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_WR
#define REG_WR( scope, inst, reg, val ) \
  REG_WRITE( reg_##scope##_##reg, \
             (inst) + REG_WR_ADDR_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_VECT
#define REG_RD_VECT( scope, inst, reg, index ) \
  REG_READ( reg_##scope##_##reg, \
            (inst) + REG_RD_ADDR_##scope##_##reg + \
	    (index) * STRIDE_##scope##_##reg )
#endif

#ifndef REG_WR_VECT
#define REG_WR_VECT( scope, inst, reg, index, val ) \
  REG_WRITE( reg_##scope##_##reg, \
             (inst) + REG_WR_ADDR_##scope##_##reg + \
	     (index) * STRIDE_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_INT
#define REG_RD_INT( scope, inst, reg ) \
  REG_READ( int, (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_WR_INT
#define REG_WR_INT( scope, inst, reg, val ) \
  REG_WRITE( int, (inst) + REG_WR_ADDR_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_INT_VECT
#define REG_RD_INT_VECT( scope, inst, reg, index ) \
  REG_READ( int, (inst) + REG_RD_ADDR_##scope##_##reg + \
	    (index) * STRIDE_##scope##_##reg )
#endif

#ifndef REG_WR_INT_VECT
#define REG_WR_INT_VECT( scope, inst, reg, index, val ) \
  REG_WRITE( int, (inst) + REG_WR_ADDR_##scope##_##reg + \
	     (index) * STRIDE_##scope##_##reg, (val) )
#endif

#ifndef REG_TYPE_CONV
#define REG_TYPE_CONV( type, orgtype, val ) \
  ( { union { orgtype o; type n; } r; r.o = val; r.n; } )
#endif

#ifndef reg_page_size
#define reg_page_size 8192
#endif

#ifndef REG_ADDR
#define REG_ADDR( scope, inst, reg ) \
  ( (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_ADDR_VECT
#define REG_ADDR_VECT( scope, inst, reg, index ) \
  ( (inst) + REG_RD_ADDR_##scope##_##reg + \
    (index) * STRIDE_##scope##_##reg )
#endif

/* C-code for register scope bif_core */

/* Register rw_grp1_cfg, scope bif_core, type rw */
typedef struct {
  unsigned int lw        : 6;
  unsigned int ew        : 3;
  unsigned int zw        : 3;
  unsigned int aw        : 2;
  unsigned int dw        : 2;
  unsigned int ewb       : 2;
  unsigned int bw        : 1;
  unsigned int wr_extend : 1;
  unsigned int erc_en    : 1;
  unsigned int mode      : 1;
  unsigned int dummy1    : 10;
} reg_bif_core_rw_grp1_cfg;
#define REG_RD_ADDR_bif_core_rw_grp1_cfg 0
#define REG_WR_ADDR_bif_core_rw_grp1_cfg 0

/* Register rw_grp2_cfg, scope bif_core, type rw */
typedef struct {
  unsigned int lw        : 6;
  unsigned int ew        : 3;
  unsigned int zw        : 3;
  unsigned int aw        : 2;
  unsigned int dw        : 2;
  unsigned int ewb       : 2;
  unsigned int bw        : 1;
  unsigned int wr_extend : 1;
  unsigned int erc_en    : 1;
  unsigned int mode      : 1;
  unsigned int dummy1    : 10;
} reg_bif_core_rw_grp2_cfg;
#define REG_RD_ADDR_bif_core_rw_grp2_cfg 4
#define REG_WR_ADDR_bif_core_rw_grp2_cfg 4

/* Register rw_grp3_cfg, scope bif_core, type rw */
typedef struct {
  unsigned int lw         : 6;
  unsigned int ew         : 3;
  unsigned int zw         : 3;
  unsigned int aw         : 2;
  unsigned int dw         : 2;
  unsigned int ewb        : 2;
  unsigned int bw         : 1;
  unsigned int wr_extend  : 1;
  unsigned int erc_en     : 1;
  unsigned int mode       : 1;
  unsigned int dummy1     : 2;
  unsigned int gated_csp0 : 2;
  unsigned int gated_csp1 : 2;
  unsigned int gated_csp2 : 2;
  unsigned int gated_csp3 : 2;
} reg_bif_core_rw_grp3_cfg;
#define REG_RD_ADDR_bif_core_rw_grp3_cfg 8
#define REG_WR_ADDR_bif_core_rw_grp3_cfg 8

/* Register rw_grp4_cfg, scope bif_core, type rw */
typedef struct {
  unsigned int lw         : 6;
  unsigned int ew         : 3;
  unsigned int zw         : 3;
  unsigned int aw         : 2;
  unsigned int dw         : 2;
  unsigned int ewb        : 2;
  unsigned int bw         : 1;
  unsigned int wr_extend  : 1;
  unsigned int erc_en     : 1;
  unsigned int mode       : 1;
  unsigned int dummy1     : 4;
  unsigned int gated_csp4 : 2;
  unsigned int gated_csp5 : 2;
  unsigned int gated_csp6 : 2;
} reg_bif_core_rw_grp4_cfg;
#define REG_RD_ADDR_bif_core_rw_grp4_cfg 12
#define REG_WR_ADDR_bif_core_rw_grp4_cfg 12

/* Register rw_sdram_cfg_grp0, scope bif_core, type rw */
typedef struct {
  unsigned int bank_sel : 5;
  unsigned int ca       : 3;
  unsigned int type     : 1;
  unsigned int bw       : 1;
  unsigned int sh       : 3;
  unsigned int wmm      : 1;
  unsigned int sh16     : 1;
  unsigned int grp_sel  : 5;
  unsigned int dummy1   : 12;
} reg_bif_core_rw_sdram_cfg_grp0;
#define REG_RD_ADDR_bif_core_rw_sdram_cfg_grp0 16
#define REG_WR_ADDR_bif_core_rw_sdram_cfg_grp0 16

/* Register rw_sdram_cfg_grp1, scope bif_core, type rw */
typedef struct {
  unsigned int bank_sel : 5;
  unsigned int ca       : 3;
  unsigned int type     : 1;
  unsigned int bw       : 1;
  unsigned int sh       : 3;
  unsigned int wmm      : 1;
  unsigned int sh16     : 1;
  unsigned int dummy1   : 17;
} reg_bif_core_rw_sdram_cfg_grp1;
#define REG_RD_ADDR_bif_core_rw_sdram_cfg_grp1 20
#define REG_WR_ADDR_bif_core_rw_sdram_cfg_grp1 20

/* Register rw_sdram_timing, scope bif_core, type rw */
typedef struct {
  unsigned int cl    : 3;
  unsigned int rcd   : 3;
  unsigned int rp    : 3;
  unsigned int rc    : 2;
  unsigned int dpl   : 2;
  unsigned int pde   : 1;
  unsigned int ref   : 2;
  unsigned int cpd   : 1;
  unsigned int sdcke : 1;
  unsigned int sdclk : 1;
  unsigned int dummy1 : 13;
} reg_bif_core_rw_sdram_timing;
#define REG_RD_ADDR_bif_core_rw_sdram_timing 24
#define REG_WR_ADDR_bif_core_rw_sdram_timing 24

/* Register rw_sdram_cmd, scope bif_core, type rw */
typedef struct {
  unsigned int cmd      : 3;
  unsigned int mrs_data : 15;
  unsigned int dummy1   : 14;
} reg_bif_core_rw_sdram_cmd;
#define REG_RD_ADDR_bif_core_rw_sdram_cmd 28
#define REG_WR_ADDR_bif_core_rw_sdram_cmd 28

/* Register rs_sdram_ref_stat, scope bif_core, type rs */
typedef struct {
  unsigned int ok : 1;
  unsigned int dummy1 : 31;
} reg_bif_core_rs_sdram_ref_stat;
#define REG_RD_ADDR_bif_core_rs_sdram_ref_stat 32

/* Register r_sdram_ref_stat, scope bif_core, type r */
typedef struct {
  unsigned int ok : 1;
  unsigned int dummy1 : 31;
} reg_bif_core_r_sdram_ref_stat;
#define REG_RD_ADDR_bif_core_r_sdram_ref_stat 36


/* Constants */
enum {
  regk_bif_core_bank2                      = 0x00000000,
  regk_bif_core_bank4                      = 0x00000001,
  regk_bif_core_bit10                      = 0x0000000a,
  regk_bif_core_bit11                      = 0x0000000b,
  regk_bif_core_bit12                      = 0x0000000c,
  regk_bif_core_bit13                      = 0x0000000d,
  regk_bif_core_bit14                      = 0x0000000e,
  regk_bif_core_bit15                      = 0x0000000f,
  regk_bif_core_bit16                      = 0x00000010,
  regk_bif_core_bit17                      = 0x00000011,
  regk_bif_core_bit18                      = 0x00000012,
  regk_bif_core_bit19                      = 0x00000013,
  regk_bif_core_bit20                      = 0x00000014,
  regk_bif_core_bit21                      = 0x00000015,
  regk_bif_core_bit22                      = 0x00000016,
  regk_bif_core_bit23                      = 0x00000017,
  regk_bif_core_bit24                      = 0x00000018,
  regk_bif_core_bit25                      = 0x00000019,
  regk_bif_core_bit26                      = 0x0000001a,
  regk_bif_core_bit27                      = 0x0000001b,
  regk_bif_core_bit28                      = 0x0000001c,
  regk_bif_core_bit29                      = 0x0000001d,
  regk_bif_core_bit9                       = 0x00000009,
  regk_bif_core_bw16                       = 0x00000001,
  regk_bif_core_bw32                       = 0x00000000,
  regk_bif_core_bwe                        = 0x00000000,
  regk_bif_core_cwe                        = 0x00000001,
  regk_bif_core_e15us                      = 0x00000001,
  regk_bif_core_e7800ns                    = 0x00000002,
  regk_bif_core_grp0                       = 0x00000000,
  regk_bif_core_grp1                       = 0x00000001,
  regk_bif_core_mrs                        = 0x00000003,
  regk_bif_core_no                         = 0x00000000,
  regk_bif_core_none                       = 0x00000000,
  regk_bif_core_nop                        = 0x00000000,
  regk_bif_core_off                        = 0x00000000,
  regk_bif_core_pre                        = 0x00000002,
  regk_bif_core_r_sdram_ref_stat_default   = 0x00000001,
  regk_bif_core_rd                         = 0x00000002,
  regk_bif_core_ref                        = 0x00000001,
  regk_bif_core_rs_sdram_ref_stat_default  = 0x00000001,
  regk_bif_core_rw_grp1_cfg_default        = 0x000006cf,
  regk_bif_core_rw_grp2_cfg_default        = 0x000006cf,
  regk_bif_core_rw_grp3_cfg_default        = 0x000006cf,
  regk_bif_core_rw_grp4_cfg_default        = 0x000006cf,
  regk_bif_core_rw_sdram_cfg_grp1_default  = 0x00000000,
  regk_bif_core_slf                        = 0x00000004,
  regk_bif_core_wr                         = 0x00000001,
  regk_bif_core_yes                        = 0x00000001
};
#endif /* __bif_core_defs_h */
