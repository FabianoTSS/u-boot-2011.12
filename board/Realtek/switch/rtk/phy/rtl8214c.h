/*
 * Copyright(c) Realtek Semiconductor Corporation, 2015
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL8218 PHY driver.
 *
 * Feature : RTL8214C PHY driver
 *
 */

#ifndef	__RTL8214C_H__
#define	__RTL8214C_H__

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
extern void rtl8214c_config(Tuint8 macId);
extern int rtl8214c_linkDownPowerSavingEnable_set(int port, int enable);
extern int rtl8214c_greenEnable_set(int port, int enable);
extern int rtl8214c_crossOverMode_set(int port, int mode);

#endif	/* __RTL8214C_H__ */

