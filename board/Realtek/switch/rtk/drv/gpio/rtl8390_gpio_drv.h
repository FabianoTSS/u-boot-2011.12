
#ifndef ____RTL8390_GPIO_DRV_H____
#define ____RTL8390_GPIO_DRV_H____

/*
 * Include Files
 */
#include <rtk/drv/gpio/gpio.h>

extern void rtl8390_intGpio_DirSet(uint32 pin, drv_gpio_direction_t dir);
extern void rtl8390_intGpio_pindata_get(uint32 pin, uint32 *pBitData);
extern void rtl8390_intGpio_pindata_set(uint32 pin, uint32 bitData);

#endif	/* ____RTL8390_GPIO_DRV_H____ */

