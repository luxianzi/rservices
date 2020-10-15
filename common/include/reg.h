/*
 * IOFPGA Register Map & Definition
 *
 * Author: Lu Xianzi, cosine@126.com
 *
 * Copyright (c) 2014
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef _IOFPGA_REG_H_
#define _IOFPGA_REG_H_

struct st_iofpga_ic_reg
{
	/* 0x00 */
	unsigned char pid;
	unsigned char spid;
	unsigned short ver;
	unsigned short veryear;
	unsigned short verdate;
	unsigned short token;
	unsigned short test;
	unsigned char gcon;
	unsigned char gint;
	unsigned char ledcon;
	unsigned char fcount;
	/* 0x10 */
	unsigned char intmask[8];
	unsigned char intflag[8];
	/* 0x20 */
	unsigned char func[64];
	/* 0x60 */
	unsigned char foffset[64];
};

struct st_iofpga_clk_reg
{
	/* 0x00 */
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char count;
	unsigned char intmask;
	unsigned char intflag;
	unsigned char syncout;
	unsigned char rsvd0[4];
	unsigned char con[4];
	/* 0x10 */
	unsigned short value[4];
	unsigned char rsvd1[8];
	/* 0x20 */
	unsigned short phase[4];
	unsigned char rsvd2[8];
};

struct st_iofpga_di_reg
{
	/* 0x00 */
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char pcount;
	unsigned char hcount;
	unsigned char hpen[2];
	unsigned char mode;
	unsigned char rsvd0[7];
	/* 0x10 */
	unsigned char pintmask[16];
	/* 0x20 */
	unsigned char hintmask[4];
	unsigned char pintflag[8];
	unsigned char hintflag[2];
	unsigned char rsvd1[2];
	/* 0x30 */
	unsigned char pdata[8];
	unsigned char hdata[2];
	unsigned char rsvd2[6];
};

struct st_iofpga_do_reg
{
	/* 0x00 */
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char pcount;
	unsigned char hcount;
	unsigned char hpen[2];
	unsigned char mode;
	unsigned char rsvd0[7];
	/* 0x10 */
	unsigned char pdata[8];
	unsigned char hdata[2];
	unsigned char rsvd1[6];
};

struct st_iofpga_ai_reg
{
	/* 0x00 */
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char count;
	unsigned char bits;
	unsigned char cmp_count;
	unsigned char cmp_htsmax;
	unsigned char mode;
	unsigned char rsvd0[7];
	/* 0x10 */
	unsigned char cmp_intmask[2];
	unsigned char cmp_intflag;
	unsigned char rsvd1[13];
	/* 0x20 */
	unsigned char cmp_htsvalue[8];
	unsigned char rsvd2[8];
	/* 0x30 */
	unsigned short cmp_value[8];
	/* 0x50 */
	unsigned short data[8];
};

struct st_iofpga_ao_reg
{
	/* 0x00 */
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char count;
	unsigned char bits;
	unsigned char mode;
	unsigned char rsvd0[9];
	/* 0x10 */
	unsigned short data[8];
};

struct st_iofpga_dda_reg
{
	/* 0x00 */
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char count;
	unsigned char valclkconf;
	unsigned short freqmax;
	unsigned char databits1ms;
	unsigned char databits2ms;
	unsigned char rsvd0;
	unsigned char databits4ms;
	unsigned char databits8ms;
	unsigned char rsvd1[3];
	/* 0x10 */
	unsigned char con;
	unsigned char rsvd2[15];
	/* 0x20 */
	unsigned short data[4];
	unsigned char rsvd3[8];
};

struct st_iofpga_cap_reg
{
	/* 0x00 */
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char count;
	unsigned char bits;
	unsigned char rsvd0[10];
	/* 0x10 */
	unsigned char conf[2];
	unsigned char en;
	unsigned char clr;
	unsigned char intmask[2];
	unsigned char intflag[2];
	unsigned char rsvd1[8];
	/* 0x20 */
	unsigned short ulimit[8];
	/* 0x30 */
	unsigned short llimit[8];
	/* 0x40 */
	unsigned short data[8];
};

struct st_iofpga_imu_reg
{
	/* 0x00 */
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned short part;
	unsigned char rsvd0[10];
	/* 0x10 */
	unsigned char con;
	unsigned char access;
	unsigned char addr;
	unsigned char rdata;
	unsigned char wdata;
	unsigned char araddr;
	unsigned char arsize;
	unsigned char rsvd1[9];
	/* 0x20 */
	unsigned char ardata[32];
};

struct st_iofpga_mlnk_reg
{
	/* 0x00 */
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char max_sl;
	unsigned char rsvd0[3];
	/* 0x08 */
	unsigned char con;
	unsigned char max_rty;
	unsigned char sl_cnt;
	unsigned char rsvd1;
	unsigned short sl_sts;
	unsigned short sl_rty;
	/* 0x10 */
	unsigned char sl_addr[32];
};

struct st_iofpga_sc_0_0_reg
{
	/* 0x00 */
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char rsvd0[12];
	/* 0x10 */
	unsigned char gint;
	unsigned char intflag;
	unsigned char clk_intflag;
	unsigned char hsi_intflag;
	unsigned char pdi_intflag[2];
	unsigned char aicmp_intflag;
	unsigned char hio;
	/* 0x18 */
	unsigned char pio[2];
	unsigned short aio[4];
	unsigned short mlnk_sl_sts;
	unsigned short mlnk_sl_rty;
	unsigned short dda_data[4];
	unsigned short cap_data[4];
	unsigned char rsvd1[2];
};

struct st_iofpga_func_info
{
	unsigned char id;
	unsigned char rsvd0;
	unsigned short info_offset;
	unsigned short conf_offset;
	unsigned short data_offset;
	unsigned short reg_offset;
	unsigned char rsvd1[6];
};

struct st_iofpga_info
{
	/* 0x00 */
	unsigned char pid;
	unsigned char spid;
	unsigned short ver;
	unsigned short veryear;
	unsigned short verdate;
	unsigned short token;
	unsigned char rsvd0[6];
	/* 0x10 */
	unsigned char fcount;
	unsigned char rsvd1[15];
	/* 0x20 */
	struct st_iofpga_func_info funcs[64];
};

union st_iofpga_info_page
{
	struct st_iofpga_info regs;
	unsigned char data[4096];
};

struct st_iofpga_clk_info
{
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char count;
	unsigned char rsvd;
	unsigned short idly_min;
	unsigned short idly_avg;
	unsigned short idly_max;
	unsigned short ndly;
};

struct st_iofpga_di_info
{
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char pcount;
	unsigned char hcount;
	unsigned short fifo_depth;
	unsigned short fifo_aethrs;
	unsigned short fifo_afthrs;
};

struct st_iofpga_do_info
{
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char pcount;
	unsigned char hcount;
	unsigned short fifo_depth;
	unsigned short fifo_aethrs;
	unsigned short fifo_afthrs;
};

struct st_iofpga_ai_info
{
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char count;
	unsigned char bits;
	unsigned char cmp_count;
	unsigned char cmp_htsmax;
	unsigned short fifo_depth;
	unsigned short fifo_aethrs;
	unsigned short fifo_afthrs;
};

struct st_iofpga_ao_info
{
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char count;
	unsigned char bits;
	unsigned short fifo_depth;
	unsigned short fifo_aethrs;
	unsigned short fifo_afthrs;
};

struct st_iofpga_dda_info
{
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char count;
	unsigned char valclkconf;
	unsigned short freqmax;
	unsigned char databits1ms;
	unsigned char databits2ms;
	unsigned char rsvd0;
	unsigned char databits4ms;
	unsigned char databits8ms;
	unsigned char rsvd1[3];
	unsigned short fifo_depth;
	unsigned short fifo_aethrs;
	unsigned short fifo_afthrs;
};

struct st_iofpga_cap_info
{
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char count;
	unsigned char bits;
	unsigned short fifo_depth;
	unsigned short fifo_aethrs;
	unsigned short fifo_afthrs;
};

struct st_iofpga_imu_info
{
	/* 0x00 */
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned short part;
	unsigned short fifo_depth;
	unsigned short fifo_aethrs;
	unsigned short fifo_afthrs;
	unsigned char rsvd0[4];
	/* 0x10 */
	unsigned char has_acc;
	unsigned char rsvd1[3];
	short acc_offsx;
	short acc_offsy;
	short acc_offsz;
	short acc_sensx;
	short acc_sensy;
	short acc_sensz;
	/* 0x20 */
	unsigned char has_gyro;
	unsigned char rsvd2[3];
	short gyro_offsx;
	short gyro_offsy;
	short gyro_offsz;
	short gyro_sensx;
	short gyro_sensy;
	short gyro_sensz;
	/* 0x30 */
	unsigned char has_mag;
	unsigned char rsvd3[3];
	short mag_offsx;
	short mag_offsy;
	short mag_offsz;
	short mag_sensx;
	short mag_sensy;
	short mag_sensz;
	/* 0x40 */
	unsigned char has_temp;
	unsigned char rsvd4;
	short temp_offs;
	short temp_sens;
	unsigned char rsvd5[10];
	/* 0x50 */
	unsigned char has_baro;
	unsigned char rsvd6;
	short baro_offs;
	short baro_sens;
	unsigned char rsvd7[10];
};

struct st_iofpga_mlnk_info
{
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
	unsigned char max_sl;
	unsigned char rsvd0;
	unsigned short fifo_rdepth;
	unsigned short fifo_raethrs;
	unsigned short fifo_rafthrs;
	unsigned short fifo_tdepth;
	unsigned short fifo_taethrs;
	unsigned short fifo_tafthrs;
};

struct st_iofpga_sc_0_0_info
{
	unsigned char ipid;
	unsigned char sipid;
	unsigned short ver;
};

struct st_iofpga_conf
{
	unsigned int reconf;
	unsigned int en;
};

union st_iofpga_conf_page
{
	struct st_iofpga_conf regs;
	unsigned char data[4096];
};

struct st_iofpga_clk_conf
{
	unsigned char intmask;
	unsigned char syncout;
	unsigned char con[8];
};

struct st_iofpga_di_conf
{
	unsigned char hpen[2];
	unsigned char mode;
	unsigned char pintmask[16];
	unsigned char hintmask[4];
};

struct st_iofpga_do_conf
{
	unsigned char hpen[2];
	unsigned char mode;
};

struct st_iofpga_ai_conf
{
	unsigned char mode;
	unsigned char rsvd0[3];
	unsigned char cmp_intmask[2];
	unsigned char rsvd1[2];
	unsigned char cmp_htsvalue[16];
};

struct st_iofpga_ao_conf
{
	unsigned char mode;
};

struct st_iofpga_dda_conf
{
	unsigned char con;
};

struct st_iofpga_cap_conf
{
	/* 0x00 */
	unsigned char conf[2];
	unsigned char en;
	unsigned char clr;
	unsigned char intmask[2];
	unsigned char rsvd[10];
	/* 0x10 */
	unsigned short ulimit[8];
	/* 0x20 */
	unsigned short llimit[8];
};

struct st_iofpga_imu_conf
{
	/* 0x00 */
	unsigned char con;
	unsigned char araddr;
	unsigned char arsize;
	unsigned char rsvd0[13];
	/* 0x10 */
	unsigned char sdiv;
	unsigned char conf;
	unsigned char gyro_conf;
	unsigned char acc_conf[2];
};

struct st_iofpga_mlnk_conf
{
	unsigned char con;
	unsigned char max_rty;
	unsigned char sl_cnt;
	unsigned char sl_addr[32];
};

union st_iofpga_data_page
{
	unsigned char data[16384];
};

struct st_iofpga_segled_data
{
	unsigned char dot;
	unsigned char string[8];
	unsigned char rsvd[7];
};

struct st_iofpga_di_data
{
	/* 0x00 */
	unsigned char pfifo_status;
	unsigned char hfifo_status;
	unsigned short pfifo_len;
	unsigned short hfifo_len;
	unsigned char rsvd0[10];
	/* 0x10 */	
	unsigned char pintflag[8];
	unsigned char hinfflag[2];
	unsigned char rsvd1[6];
	/* 0x20 */
	unsigned char pdata[8];
	unsigned char hdata[2];
	unsigned char rsvd2[6];
	/* 0x30 */
	unsigned char pfifo[8];
	unsigned char hfifo[2];
	unsigned char rsvd3[6];
};

struct st_iofpga_do_data
{
	/* 0x00 */
	unsigned char pfifo_status;
	unsigned char hfifo_status;
	unsigned short pfifo_len;
	unsigned short hfifo_len;
	unsigned char rsvd0[10];
	/* 0x10 */
	unsigned char pdata[8];
	unsigned char hdata[2];
	unsigned char rsvd1[6];
	/* 0x20 */
	unsigned char pfifo[8];
	unsigned char hfifo[2];
	unsigned char rsvd2[6];
};

struct st_iofpga_ai_data
{
	/* 0x00 */
	unsigned char fifo_status;
	unsigned char rsvd0;
	unsigned short fifo_len;
	unsigned char cmp_intflag;
	unsigned char rsvd1[11];
	/* 0x10 */
	unsigned short data[8];
	/* 0x20 */
	unsigned short fifo[8];
};

struct st_iofpga_ao_data
{
	/* 0x00 */
	unsigned char fifo_status;
	unsigned char rsvd0;
	unsigned short fifo_len;
	unsigned char rsvd1[12];
	/* 0x10 */
	unsigned short data[8];
	/* 0x20 */
	unsigned short fifo[8];
};

struct st_iofpga_dda_data
{
	/* 0x00 */
	unsigned char fifo_status;
	unsigned char rsvd0;
	unsigned short fifo_len;
	unsigned char rsvd1[12];
	/* 0x10 */
	unsigned short data[4];
	unsigned char rsvd2[8];
	/* 0x20 */
	unsigned short fifo[8];
};

struct st_iofpga_cap_data
{
	/* 0x00 */
	unsigned char fifo_status;
	unsigned char rsvd0;
	unsigned short fifo_len;
	unsigned char intflag[2];
	unsigned char rsvd1[10];
	/* 0x10 */
	unsigned short data[8];
	/* 0x20 */
	unsigned short fifo[8];
};

struct st_iofpga_imu_data
{
	/* 0x00 */
	unsigned char fifo_status;
	unsigned char rsvd0;
	unsigned short fifo_len;
	unsigned char rsvd1[12];
	/* 0x10 */
	unsigned char data[32];
	/* 0x30 */
	unsigned char fifo[32];
};

struct st_iofpga_imu_value
{
	short accx;
	short accy;
	short accz;
	short gyrox;
	short gyroy;
	short gyroz;
	short magx;
	short magy;
	short magz;
	short temp;
	short baro;
	char rsvd[10];
};

struct st_iofpga_mlnk_data
{
	/* 0x000 */
	unsigned char rfifo_status;
	unsigned char tfifo_status;
	unsigned short rfifo_len;
	unsigned short tfifo_len;
	unsigned short sl_sts;
	unsigned short sl_rty;
	unsigned char rsvd1[6];
	/* 0x010 */
	unsigned char rdata[32][32];
	/* 0x410 */
	unsigned char tdata[32][32];
	/* 0x810 */
	unsigned char rfifo[32][32];
	/* 0xc10 */
	unsigned char tfifo[32][32];
};

#define IOFPGA_IC_ADDR(reg)					((unsigned int)&(((struct st_iofpga_ic_reg *)0)->reg))
#define IOFPGA_IC_SIZE(reg)					sizeof(((struct st_iofpga_ic_reg *)0)->reg)
#define IOFPGA_CLK_ADDR(base, reg)			((unsigned int)&(((struct st_iofpga_clk_reg *)base)->reg))
#define IOFPGA_CLK_SIZE(reg)				sizeof(((struct st_iofpga_clk_reg *)0)->reg)
#define IOFPGA_DI_ADDR(base, reg)			((unsigned int)&(((struct st_iofpga_di_reg *)base)->reg))
#define IOFPGA_DI_SIZE(reg)					sizeof(((struct st_iofpga_di_reg *)0)->reg)
#define IOFPGA_DO_ADDR(base, reg)			((unsigned int)&(((struct st_iofpga_do_reg *)base)->reg))
#define IOFPGA_DO_SIZE(reg)					sizeof(((struct st_iofpga_do_reg *)0)->reg)
#define IOFPGA_AI_ADDR(base, reg)			((unsigned int)&(((struct st_iofpga_ai_reg *)base)->reg))
#define IOFPGA_AI_SIZE(reg)					sizeof(((struct st_iofpga_ai_reg *)0)->reg)
#define IOFPGA_AO_ADDR(base, reg)			((unsigned int)&(((struct st_iofpga_ao_reg *)base)->reg))
#define IOFPGA_AO_SIZE(reg)					sizeof(((struct st_iofpga_ao_reg *)0)->reg)
#define IOFPGA_DDA_ADDR(base, reg)			((unsigned int)&(((struct st_iofpga_dda_reg *)base)->reg))
#define IOFPGA_DDA_SIZE(reg)				sizeof(((struct st_iofpga_dda_reg *)0)->reg)
#define IOFPGA_CAP_ADDR(base, reg)			((unsigned int)&(((struct st_iofpga_cap_reg *)base)->reg))
#define IOFPGA_CAP_SIZE(reg)				sizeof(((struct st_iofpga_cap_reg *)0)->reg)
#define IOFPGA_IMU_ADDR(base, reg)			((unsigned int)&(((struct st_iofpga_imu_reg *)base)->reg))
#define IOFPGA_IMU_SIZE(reg)				sizeof(((struct st_iofpga_imu_reg *)0)->reg)
#define IOFPGA_MLNK_ADDR(base, reg)			((unsigned int)&(((struct st_iofpga_mlnk_reg *)base)->reg))
#define IOFPGA_MLNK_SIZE(reg)				sizeof(((struct st_iofpga_mlnk_reg *)0)->reg)
#define IOFPGA_SC_0_0_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_sc_0_0_reg *)base)->reg))
#define IOFPGA_SC_0_0_SIZE(reg)				sizeof(((struct st_iofpga_sc_0_0_reg *)0)->reg)

#define IOFPGA_CONFPAGE_ADDR(reg)			((unsigned int)&(((union st_iofpga_conf_page *)0)->reg))
#define IOFPGA_CONFPAGE_DATA_ADDR			((unsigned int)&(((union st_iofpga_conf_page *)0)->regs.en) + sizeof(unsigned int))

#define IOFPGA_PID							0x07
#define IOFPGA_SPID							0x01
#define IOFPGA_TOKEN						0x55aa

#define IOFPGA_OFFSET_2_ADDR(offset)		(offset << 4)
#define IOFPGA_PTR_2_CLK_REG(ptr)			((struct st_iofpga_clk_reg *)(ptr))
#define IOFPGA_PTR_2_DI_REG(ptr)			((struct st_iofpga_di_reg *)(ptr))
#define IOFPGA_PTR_2_DO_REG(ptr)			((struct st_iofpga_do_reg *)(ptr))
#define IOFPGA_PTR_2_AI_REG(ptr)			((struct st_iofpga_ai_reg *)(ptr))
#define IOFPGA_PTR_2_AO_REG(ptr)			((struct st_iofpga_ao_reg *)(ptr))
#define IOFPGA_PTR_2_DDA_REG(ptr)			((struct st_iofpga_dda_reg *)(ptr))
#define IOFPGA_PTR_2_CAP_REG(ptr)			((struct st_iofpga_cap_reg *)(ptr))
#define IOFPGA_PTR_2_IMU_REG(ptr)			((struct st_iofpga_imu_reg *)(ptr))
#define IOFPGA_PTR_2_MLNK_REG(ptr)			((struct st_iofpga_mlnk_reg *)(ptr))
#define IOFPGA_PTR_2_SC_0_0_REG(ptr)		((struct st_iofpga_sc_0_0_reg *)(ptr))
#define IOFPGA_PTR_2_CLK_INFO(ptr)			((struct st_iofpga_clk_info *)(ptr))
#define IOFPGA_PTR_2_DI_INFO(ptr)			((struct st_iofpga_di_info *)(ptr))
#define IOFPGA_PTR_2_DO_INFO(ptr)			((struct st_iofpga_do_info *)(ptr))
#define IOFPGA_PTR_2_AI_INFO(ptr)			((struct st_iofpga_ai_info *)(ptr))
#define IOFPGA_PTR_2_AO_INFO(ptr)			((struct st_iofpga_ao_info *)(ptr))
#define IOFPGA_PTR_2_DDA_INFO(ptr)			((struct st_iofpga_dda_info *)(ptr))
#define IOFPGA_PTR_2_CAP_INFO(ptr)			((struct st_iofpga_cap_info *)(ptr))
#define IOFPGA_PTR_2_IMU_INFO(ptr)			((struct st_iofpga_imu_info *)(ptr))
#define IOFPGA_PTR_2_MLNK_INFO(ptr)			((struct st_iofpga_mlnk_info *)(ptr))
#define IOFPGA_PTR_2_SC_0_0_INFO(ptr)		((struct st_iofpga_sc_0_0_info *)(ptr))
#define IOFPGA_PTR_2_CLK_CONF(ptr)			((struct st_iofpga_clk_conf *)(ptr))
#define IOFPGA_PTR_2_DI_CONF(ptr)			((struct st_iofpga_di_conf *)(ptr))
#define IOFPGA_PTR_2_DO_CONF(ptr)			((struct st_iofpga_do_conf *)(ptr))
#define IOFPGA_PTR_2_AI_CONF(ptr)			((struct st_iofpga_ai_conf *)(ptr))
#define IOFPGA_PTR_2_AO_CONF(ptr)			((struct st_iofpga_ao_conf *)(ptr))
#define IOFPGA_PTR_2_DDA_CONF(ptr)			((struct st_iofpga_dda_conf *)(ptr))
#define IOFPGA_PTR_2_CAP_CONF(ptr)			((struct st_iofpga_cap_conf *)(ptr))
#define IOFPGA_PTR_2_IMU_CONF(ptr)			((struct st_iofpga_imu_conf *)(ptr))
#define IOFPGA_PTR_2_MLNK_CONF(ptr)			((struct st_iofpga_mlnk_conf *)(ptr))
#define IOFPGA_PTR_2_DI_DATA(ptr)			((struct st_iofpga_di_data *)(ptr))
#define IOFPGA_PTR_2_DO_DATA(ptr)			((struct st_iofpga_do_data *)(ptr))
#define IOFPGA_PTR_2_AI_DATA(ptr)			((struct st_iofpga_ai_data *)(ptr))
#define IOFPGA_PTR_2_AO_DATA(ptr)			((struct st_iofpga_ao_data *)(ptr))
#define IOFPGA_PTR_2_DDA_DATA(ptr)			((struct st_iofpga_dda_data *)(ptr))
#define IOFPGA_PTR_2_CAP_DATA(ptr)			((struct st_iofpga_cap_data *)(ptr))
#define IOFPGA_PTR_2_IMU_DATA(ptr)			((struct st_iofpga_imu_data *)(ptr))
#define IOFPGA_PTR_2_MLNK_DATA(ptr)			((struct st_iofpga_mlnk_data *)(ptr))

#define IOFPGA_SEGLED_DATA_ADDR(base, reg)	((unsigned int)&(((struct st_iofpga_segled_data *)base)->reg))
#define IOFPGA_SEGLED_DATA_SIZE(reg)		sizeof(((struct st_iofpga_segled_data *)0)->reg)
#define IOFPGA_CLK_CONF_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_clk_conf *)((int)base))->reg))
#define IOFPGA_CLK_CONF_SIZE(reg)			sizeof(((struct st_iofpga_clk_conf *)0)->reg)
#define IOFPGA_DI_CONF_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_di_conf *)((int)base))->reg))
#define IOFPGA_DI_CONF_SIZE(reg)			sizeof(((struct st_iofpga_di_conf *)0)->reg)
#define IOFPGA_DI_DATA_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_di_data *)((int)base))->reg))
#define IOFPGA_DI_DATA_SIZE(reg)			sizeof(((struct st_iofpga_di_data *)0)->reg)
#define IOFPGA_DO_CONF_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_do_conf *)((int)base))->reg))
#define IOFPGA_DO_CONF_SIZE(reg)			sizeof(((struct st_iofpga_do_conf *)0)->reg)
#define IOFPGA_DO_DATA_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_do_data *)((int)base))->reg))
#define IOFPGA_DO_DATA_SIZE(reg)			sizeof(((struct st_iofpga_do_data *)0)->reg)
#define IOFPGA_AI_CONF_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_ai_conf *)((int)base))->reg))
#define IOFPGA_AI_CONF_SIZE(reg)			sizeof(((struct st_iofpga_ai_conf *)0)->reg)
#define IOFPGA_AI_DATA_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_ai_data *)((int)base))->reg))
#define IOFPGA_AI_DATA_SIZE(reg)			sizeof(((struct st_iofpga_ai_data *)0)->reg)
#define IOFPGA_AO_CONF_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_ao_conf *)((int)base))->reg))
#define IOFPGA_AO_CONF_SIZE(reg)			sizeof(((struct st_iofpga_ao_conf *)0)->reg)
#define IOFPGA_AO_DATA_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_ao_data *)((int)base))->reg))
#define IOFPGA_AO_DATA_SIZE(reg)			sizeof(((struct st_iofpga_ao_data *)0)->reg)
#define IOFPGA_DDA_CONF_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_dda_conf *)((int)base))->reg))
#define IOFPGA_DDA_CONF_SIZE(reg)			sizeof(((struct st_iofpga_dda_conf *)0)->reg)
#define IOFPGA_DDA_DATA_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_dda_data *)((int)base))->reg))
#define IOFPGA_DDA_DATA_SIZE(reg)			sizeof(((struct st_iofpga_dda_data *)0)->reg)
#define IOFPGA_CAP_CONF_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_cap_conf *)((int)base))->reg))
#define IOFPGA_CAP_CONF_SIZE(reg)			sizeof(((struct st_iofpga_cap_conf *)0)->reg)
#define IOFPGA_CAP_DATA_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_cap_data *)((int)base))->reg))
#define IOFPGA_CAP_DATA_SIZE(reg)			sizeof(((struct st_iofpga_cap_data *)0)->reg)
#define IOFPGA_IMU_CONF_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_imu_conf *)((int)base))->reg))
#define IOFPGA_IMU_CONF_SIZE(reg)			sizeof(((struct st_iofpga_imu_conf *)0)->reg)
#define IOFPGA_IMU_DATA_ADDR(base, reg)		((unsigned int)&(((struct st_iofpga_imu_data *)((int)base))->reg))
#define IOFPGA_IMU_DATA_SIZE(reg)			sizeof(((struct st_iofpga_imu_data *)0)->reg)
#define IOFPGA_MLNK_CONF_ADDR(base, reg)	((unsigned int)&(((struct st_iofpga_mlnk_conf *)((int)base))->reg))
#define IOFPGA_MLNK_CONF_SIZE(reg)			sizeof(((struct st_iofpga_mlnk_conf *)0)->reg)
#define IOFPGA_MLNK_DATA_ADDR(base, reg)	((unsigned int)&(((struct st_iofpga_mlnk_data *)((int)base))->reg))
#define IOFPGA_MLNK_DATA_SIZE(reg)			sizeof(((struct st_iofpga_mlnk_data *)0)->reg)
#define IOFPGA_IMU_VALUE_ADDR(base, reg)	((unsigned int)&(((struct st_iofpga_imu_value *)((int)base))->reg))

#define IOFPGA_MLNK0_RAWDATA_BASE			0
#define IOFPGA_MLNK1_RAWDATA_BASE			(32 * 32)

#define IOFPGA_HW_FUNCID_CLK				0x01
#define IOFPGA_HW_FUNCID_DI					0x02
#define IOFPGA_HW_FUNCID_DO					0x03
#define IOFPGA_HW_FUNCID_AI					0x04
#define IOFPGA_HW_FUNCID_AO					0x05
#define IOFPGA_HW_FUNCID_DDA				0x06
#define IOFPGA_HW_FUNCID_CAP				0x07
#define IOFPGA_HW_FUNCID_IMU				0x08
#define IOFPGA_HW_FUNCID_MLNK0				0x10
#define IOFPGA_HW_FUNCID_MLNK1				0x11
#define IOFPGA_HW_FUNCID_SC_0_0				0x80

#define IOFPGA_CLK_CYCLE_1MS				0
#define IOFPGA_CLK_CYCLE_2MS				1
#define IOFPGA_CLK_CYCLE_3MS				2
#define IOFPGA_CLK_CYCLE_4MS				3
#define IOFPGA_CLK_CYCLE_8MS				4
#define IOFPGA_CLK_CYCLE_16MS				5
#define IOFPGA_CLK_CYCLE_32MS				6
#define IOFPGA_CLK_CYCLE_64MS				7

#define IOFPGA_DDA_MAX_FREQ					512000
#define IOFPGA_DDA_SIGN						(1 << 15)

#endif
