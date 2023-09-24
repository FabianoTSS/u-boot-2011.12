/*
 * Copyright(c) Realtek Semiconductor Corporation, 2012
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL8208 PHY driver.
 *
 * Feature : RTL8295R PHY driver
 *
 */

#ifndef	__RTL8295R_H__
#define	__RTL8295R_H__

/*
 * Include Files
 */

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */
extern void rtl8295r_phyPortPowerOff(int32 port);
extern void rtl8295r_phyPortPowerOn(int32 port);
extern void rtl8295r_config(Tuint8 phyid);

#endif	/* __RTL8295R_H__ */

