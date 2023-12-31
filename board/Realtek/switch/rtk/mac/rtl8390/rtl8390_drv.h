/*
 * Copyright(c) Realtek Semiconductor Corporation, 2012
 * All rights reserved.
 *
 * Purpose : Related definition of the RTL8390 driver.
 *
 * Feature : RTL8390 driver
 *
 */

#ifndef __RTL8390_DRV_H__
#define __RTL8390_DRV_H__

/*
 * Include Files
 */
#include <rtk_switch.h>

/*
 * Symbol Definition
 */
#define SERDES_SET              rtl839x_serdes_patch_set

/*
 * Function Declaration
 */
extern int rtl8390_setPhyReg(int portid, int page, int reg, unsigned int val);
extern int rtl8390_getPhyReg(int portid, int page, int reg, unsigned int *val);
extern int rtl8390_setPhyRegByMask(unsigned long long port_mask, int page, int reg, unsigned int val);
extern void rtl8390_phyPortPowerOn(int portId);
extern void rtl8390_phyPortPowerOff(int portId);
extern void rtl8390_phyPowerOn(void);
extern void rtl8390_phyPowerOff(void);
extern void rtl8390_phyReset(const rtk_switch_model_t *pModel);

extern void rtl8390_drv_macPhyPatch1(void);
extern void rtl8390_drv_macPhyPatch2(void);
extern void rtl8390_fiberRxWatchdog(void);

void rtl839x_serdes_patch_set(uint32 reg, uint32 endBit, uint32 startBit, uint32 val);

#endif  /* __RTL8390_DRV_H__ */

