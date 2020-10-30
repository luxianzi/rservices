/*
 * IOFPGA Driver Definition
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

#ifndef _IOFPGA_DRIVER_H_
#define _IOFPGA_DRIVER_H_

#include "reg.h"

#define IOFPGA_PAGE_INFO				0
#define IOFPGA_PAGE_CONF				1
#define IOFPGA_PAGE_DATA				2
#define IOFPGA_PAGE_FW					3
#define IOFPGA_PAGE_REG					4

#define IOFPGA_FUNC_INFO				0
#define IOFPGA_FUNC_CONF				1
#define IOFPGA_FUNC_DATA				2
#define IOFPGA_FUNC_FW					3
#define IOFPGA_FUNC_REG					4
#define IOFPGA_FUNC_REG8				5
#define IOFPGA_FUNC_REG16				6
#define IOFPGA_FUNC_ADDR				7

#define IOFPGA_FIFO_DEPTH				1024
#define IOFPGA_FIFO_AETHRS				(IOFPGA_FIFO_DEPTH / 8)
#define IOFPGA_FIFO_AFTHRS				(IOFPGA_FIFO_DEPTH / 8)
#define IOFPGA_FIFO_EMPTY				(1 << 0)
#define IOFPGA_FIFO_ALMOST_EMPTY		(1 << 1)
#define IOFPGA_FIFO_ALMOST_FULL			(1 << 2)
#define IOFPGA_FIFO_FULL				(1 << 3)

struct st_iofpga_reg_access
{
	unsigned short addr;
	unsigned short value;
};

typedef struct st_iofpga_reg_access iofpga_reg_access_t;

#define IOFPGA_MAGIC					'L'
#define IOFPGA_SEL_INFO					_IO(IOFPGA_MAGIC, IOFPGA_FUNC_INFO)
#define IOFPGA_SEL_CONF					_IO(IOFPGA_MAGIC, IOFPGA_FUNC_CONF)
#define IOFPGA_SEL_DATA					_IO(IOFPGA_MAGIC, IOFPGA_FUNC_DATA)
#define IOFPGA_SEL_FW					_IO(IOFPGA_MAGIC, IOFPGA_FUNC_FW)
#define IOFPGA_SEL_REG					_IO(IOFPGA_MAGIC, IOFPGA_FUNC_REG)
#define IOFPGA_RD_REG8					_IOR(IOFPGA_MAGIC, IOFPGA_FUNC_REG8, iofpga_reg_access_t)
#define IOFPGA_WR_REG8					_IOW(IOFPGA_MAGIC, IOFPGA_FUNC_REG8, iofpga_reg_access_t)
#define IOFPGA_RD_REG16					_IOR(IOFPGA_MAGIC, IOFPGA_FUNC_REG16, iofpga_reg_access_t)
#define IOFPGA_WR_REG16					_IOW(IOFPGA_MAGIC, IOFPGA_FUNC_REG16, iofpga_reg_access_t)
#define IOFPGA_RD_ADDR					_IOR(IOFPGA_MAGIC, IOFPGA_FUNC_ADDR, unsigned int)
#define IOFPGA_WR_ADDR					_IOW(IOFPGA_MAGIC, IOFPGA_FUNC_ADDR, unsigned int)

extern struct device * iofpga_device;

#define IOFPGA_CONFPAGE_RECONF_PASSWD	0x55aa0000
#define IOFPGA_CONFPAGE_EN_PASSWD		0xaa550000

#endif
