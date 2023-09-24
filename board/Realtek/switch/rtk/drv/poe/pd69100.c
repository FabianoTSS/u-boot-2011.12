#include <rtk_osal.h>
#include <config.h>
#include <common.h>
#include <command.h>
#include <rtk/drv/poe/pd69100.h>
#include <rtk/drv/poe/poe.h>
#include <rtk_reg.h>
#include "rtk_type.h"
#if defined(CONFIG_RTL8380)
#include <rtk/mac/rtl8380/rtl8380_rtk.h>
#include <rtk/mac/rtl8380/rtl8380_mdc_mdio.h>
#include <rtk/mac/rtl8380/rtl8380_swcore_reg.h>
#endif

#if defined(CONFIG_RTL8390)
#include <rtk/mac/rtl8390/rtl8390_drv.h>
#include <rtk/mac/rtl8390/rtl8390_mdc_mdio.h>
#include <rtk/mac/rtl8390/rtl8390_swcore_reg.h>
#endif
#include <rtk/drv/gpio/ext_gpio.h>
/* 
 * Symbol Definition 
 */

/* 
 * Data Declaration 
 */

/*
 * Macro Definition
 */
#ifndef REG32
#define REG32(reg)   (*(volatile unsigned int *)((unsigned int)reg))
#endif
#ifndef PORTMASK_COUNT
#define PORTMASK_COUNT(port_count)  ((0x1 << (port_count)) - 1)
#endif

#define SUCCESS 0
#define TRUE 1
#define FALSE 0

typedef enum rtk_enable_e
{
    DISABLED = 0,
    ENABLED,
    RTK_ENABLE_END
} rtk_enable_t;

typedef struct poe_portAdmin_s
{
    unsigned int port;
    rtk_enable_t     enable;
} poe_portAdmin_t;

typedef struct sys_poe_fw_s
{
    uint8 isFirmwareOk;
    uint8 majorVer;
    uint8 minorVer;
} sys_poe_fw_t;

typedef struct sys_poe_power_s
{
    uint32 maxPower;        /* mw */
    uint32 totalConsumed;   /* mw */
    uint32 totalCalculated; /* mw */
} sys_poe_power_t;

//#define POE_PD69100_MSG_RETRY               4
#define POE_PD69100_MSG_RETRY               1

#define POE_PD69100_CMD_DELAY               400     /* 400ms */

#define POE_PD69100_KEY_COMMAND             0x00
#define POE_PD69100_KEY_PROGRAM             0x01
#define POE_PD69100_KEY_REQUEST             0x02
#define POE_PD69100_KEY_TELEMOERY           0x03
#define POE_PD69100_KEY_REPORT              0x52

static void _poe_uart_empty_rxbuf(void);


#if defined(CONFIG_RTL8380)
int poe_uart_init(uint32 unit)
{
    /* GMII interface select to uart-1, default is SPI slave */
    MEM32_WRITE(SWCORE_BASE_ADDR|RTL8380_GMII_INTF_SEL_ADDR, 0x10);

    /* uart-1 initialize code */
		serial_IoCtl(BCM59111_UART_DEV,SERIAL_IO_BAUD,BCM59111_UART_BAUDRATE);
		udelay(3000);
		printf("\n init rtl8380 uart channel 1,baudrate %d",BCM59111_UART_BAUDRATE);
    return SYS_ERR_OK;
} /* end of r8380_uart_init */
#endif
#if defined(CONFIG_RTL8390)
int poe_uart_init(int unit)
{
    unsigned int  data;

    /* select to uart-1 interface, default is E-JTAG */
    data = MEM32_READ(SWCORE_BASE_ADDR | RTL8390_MAC_IF_CTRL_ADDR);
    data &= ~(RTL8390_MAC_IF_CTRL_JTAG_SEL_MASK);
    data |= (0x1 << RTL8390_MAC_IF_CTRL_JTAG_SEL_OFFSET);
    MEM32_WRITE(SWCORE_BASE_ADDR| RTL8390_MAC_IF_CTRL_ADDR, data);

    /* uart-1 initialize code */
		serial_IoCtl(BCM59111_UART_DEV,SERIAL_IO_BAUD,BCM59111_UART_BAUDRATE);
		udelay(3000);
		printf("\n init rtl8390 uart channel 1,baudrate %d",BCM59111_UART_BAUDRATE);
    return SYS_ERR_OK;
} /* end of r8390_uart_init */
#endif
static int32 poe_pd69100_enable(unsigned int enable)
{
    rtl8231_pin_status_set(8, ENABLED == enable ? 1 : 0);

    return SYS_ERR_OK;
}

static int uart_poe_get(int chan,unsigned char *c)
{
	unsigned int i;
	int result;
	
	result=SYS_ERR_FAILED;
	*c=0;
	for (i=0;i<3000;i++)
	{
		if (serial_chan_tstc(chan))
		{
			*c= serial_chan_getc(chan);
			result = SYS_ERR_OK;
			break;
		}
		else
			udelay(2);
	}
	//putc(*c);
	return result;
}

static void _poe_uart_empty_rxbuf()
{
	while (serial_chan_tstc(BCM59111_UART_DEV))
			serial_chan_getc(BCM59111_UART_DEV);
	return;
}
static int _poe_uart_message_read(unsigned char *pMsg, unsigned int byteLen)
{
    unsigned int i = 0;
    int ret,ret_cnt,ok_cnt;
    
		ret_cnt=0;
		ok_cnt=0;
    while (i < byteLen)
    {
    		ret = uart_poe_get(BCM59111_UART_DEV,&pMsg[i]);
    		if (ret != SYS_ERR_OK)
    			ret_cnt++;
    		else
    			ok_cnt++;
    		if (ret_cnt >= 10)
    		{
    			printf("\n time out cnt %d ok_cnt %d,message %d ",ret_cnt,ok_cnt,i);
    			break;
    		}
    		i++;		
    }

    return SYS_ERR_OK;
}

static int _poe_uart_message_write(unsigned char *pMsg, unsigned int byteLen)
{
    unsigned int i = 0;

    while (i < byteLen)
    		serial_chan_putc(BCM59111_UART_DEV,pMsg[i++]);

    return SYS_ERR_OK;
}
static int poe_pd69100_message_checksum(poe_pd69100_msg_t *pMsg, unsigned short *pSum)
{
    unsigned int i = 0;
    unsigned int sum = 0;
    unsigned char *data = (unsigned char *)pMsg;

    SYS_PARAM_CHK((NULL == pMsg), -1);
    SYS_PARAM_CHK((NULL == pSum), -1);

    for (i = 0; i < sizeof(poe_pd69100_msg_t); i++)
    {
        sum += data[i];
        sum &= 0xFFFF;
    }

    *pSum = (unsigned short)sum;

    return SYS_ERR_OK;
}

static int pd69100_message_read(poe_pd69100_msg_t *pMsg, unsigned int echo)
{
    unsigned int retry = 0;

    if (pMsg == 0)
    	return -1;

    do
    {
        SYS_ERR_CONTINUE(_poe_uart_message_read((unsigned char *)pMsg, sizeof(poe_pd69100_msg_t)));
        if (echo == pMsg->echo)
            break;
    } while (++retry < POE_PD69100_MSG_RETRY);

    /* echo */
    if (echo != pMsg->echo)
        return SYS_ERR_FAILED;

    return SYS_ERR_OK;
}

static int pd69100_message_write(poe_pd69100_msg_t *pMsg)
{
    SYS_PARAM_CHK((NULL == pMsg), -1);

    SYS_ERR_CHK(_poe_uart_message_write((unsigned char *)pMsg, sizeof(poe_pd69100_msg_t)));

    return SYS_ERR_OK;
}
int poe_pd69100_cmd_get(poe_pd69100_cmd_t cmd, poe_pd69100_msg_t *pMsg, ...)
{
    unsigned int data = 0;
    unsigned int retval = 0;
    unsigned int delay = 0; 
    //unsigned int is_delay = TRUE;
    unsigned int is_delay = FALSE;
    unsigned short checksum1 = 0;
    unsigned short checksum2 = 0;
    unsigned char echo = cmd & 0xFF;

    va_list args;
    poe_pd69100_msg_t msg;

    SYS_PARAM_CHK((POE_PD69100_CMD_REQUEST > cmd), -1);

    memset(&msg, 0x4E, sizeof(poe_pd69100_msg_t));
    memset(msg.checksum, 0, sizeof(msg.checksum));

    switch (cmd)
    {
        case POE_PD69100_CMD_FW_VERSION_GET:
            msg.key = POE_PD69100_KEY_REQUEST;
            msg.echo = echo;
            msg.payload[0] = 0x07;
            msg.payload[1] = 0x1E;
            msg.payload[2] = 0x21;

            break;

        case POE_PD69100_CMD_SYSTEM_STATUS_GET:
            msg.key = POE_PD69100_KEY_REQUEST;
            msg.echo = echo;
            msg.payload[0] = 0x07;
            msg.payload[1] = 0x3D;

            break;


        case POE_PD69100_CMD_SYSTEM_MASK_GET:
            msg.key = POE_PD69100_KEY_REQUEST;
            msg.echo = echo;
            msg.payload[0] = 0x07;
            msg.payload[1] = 0x2B;

            break;

        case POE_PD69100_CMD_PM_METHOD_GET:
            msg.key = POE_PD69100_KEY_REQUEST;
            msg.echo = echo;
            msg.payload[0] = 0x07;
            msg.payload[1] = 0x0B;
            msg.payload[2] = 0x5F;

            break;

        case POE_PD69100_CMD_TOTAL_POWER_GET:
            msg.key = POE_PD69100_KEY_REQUEST;
            msg.echo = echo;
            msg.payload[0] = 0x07;
            msg.payload[1] = 0x0B;
            msg.payload[2] = 0x60;

            break;

        case POE_PD69100_CMD_PORT_PRIORITY_GET:
            msg.key = POE_PD69100_KEY_REQUEST;
            msg.echo = echo;
            msg.payload[0] = 0x05;
            msg.payload[1] = 0x0A;

            va_start(args, pMsg);
            data = va_arg(args, unsigned int);
            msg.payload[2] = data & 0xFF;           /* port */
            va_end(args);

            break;

        case POE_PD69100_CMD_PORT_STATUS_GET:
            msg.key = POE_PD69100_KEY_REQUEST;
            msg.echo = echo;
            msg.payload[0] = 0x05;
            msg.payload[1] = 0x0E;

            va_start(args, pMsg);
            data = va_arg(args, unsigned int);
            msg.payload[2] = data & 0xFF;           /* port */
            va_end(args);

            break;

        case POE_PD69100_CMD_PORT_POWER_LIMIT_GET:
            msg.key = POE_PD69100_KEY_REQUEST;
            msg.echo = echo;
            msg.payload[0] = 0x05;
            msg.payload[1] = 0x0B;

            va_start(args, pMsg);
            data = va_arg(args, unsigned int);
            msg.payload[2] = data & 0xFF;           /* port */
            va_end(args);

            break;

        case POE_PD69100_CMD_PORT_POWER_MEASUREMENT_GET:
            msg.key = POE_PD69100_KEY_REQUEST;
            msg.echo = echo;
            msg.payload[0] = 0x05;
            msg.payload[1] = 0x25;

            va_start(args, pMsg);
            data = va_arg(args, unsigned int);
            msg.payload[2] = data & 0xFF;           /* port */
            va_end(args);

            break;

        default:
            return SYS_ERR_UNKNOWN;
            break;
            
    }

    /* checksum */
    SYS_ERR_CHK(poe_pd69100_message_checksum(&msg, &checksum1));
    memcpy(msg.checksum, &checksum1, sizeof(unsigned short));

		_poe_uart_empty_rxbuf();
		
    SYS_ERR_CHK(pd69100_message_write(&msg));
    //udelay(5);

    memset(&msg, 0, sizeof(poe_pd69100_msg_t));

    /* delay ? */
    if (is_delay)
    {
        delay = POE_PD69100_CMD_DELAY / 10;
        while (delay)
        {
            retval = pd69100_message_read(&msg, echo);
            if (SYS_ERR_OK == retval)
                break;

            if (--delay)
                udelay(10 * 1000);
        }
    }
    else
    {
        SYS_ERR_CHK(pd69100_message_read(&msg, echo));
    }
#define POE_PD69100_KEY_TELEMOERY           0x03
    /* TELEMOERY ? */
    if (POE_PD69100_KEY_TELEMOERY != msg.key)
    {
    		printf("\n POE get respone but key is not correct.!\n");
        return SYS_ERR_FAILED;
		}
    /* checksum ? */
    memcpy(&checksum2, msg.checksum, sizeof(unsigned short));
    memset(msg.checksum, 0, sizeof(msg.checksum));
    SYS_ERR_CHK(poe_pd69100_message_checksum(&msg, &checksum1));
    if (checksum2 != checksum1)
    {
    	printf("\n POE get respone but checksum is not correct.!\n");
      return SYS_ERR_FAILED;
		}
    memcpy(pMsg, &msg, sizeof(poe_pd69100_msg_t));

    return SYS_ERR_OK;
}

int poe_pd69100_cmd_set(poe_pd69100_cmd_t cmd, ...)
{
    unsigned int data = 0;
    unsigned int retval = 0;
    unsigned int delay = 0;
    unsigned int is_delay = TRUE;
    unsigned short checksum1 = 0;
    unsigned short checksum2 = 0;
    unsigned char echo = cmd & 0xFF;

    va_list args;
    poe_pd69100_msg_t msg;

    SYS_PARAM_CHK((POE_PD69100_CMD_REQUEST < cmd), SYS_ERR_FAILED);

    memset(&msg, 0x4E, sizeof(poe_pd69100_msg_t));
    memset(msg.checksum, 0, sizeof(msg.checksum));

    switch (cmd)
    {
        case POE_PD69100_CMD_RESET_SET:
            is_delay = FALSE;
            msg.key = POE_PD69100_KEY_COMMAND;
            msg.echo = 0xFF;
            msg.payload[0] = 0x07;
            msg.payload[1] = 0x55;
            msg.payload[2] = 0x00;
            msg.payload[3] = 0x55;
            msg.payload[4] = 0x00;
            msg.payload[5] = 0x55;

            break;

        case POE_PD69100_CMD_SYSTEM_MASK_SET:
            msg.key = POE_PD69100_KEY_COMMAND;
            msg.echo = echo;
            msg.payload[0] = 0x07;
            msg.payload[1] = 0x2B;

            va_start(args, cmd);
            data = va_arg(args, unsigned int);
            msg.payload[2] = data & 0xFF;
            va_end(args);

            break;

        case POE_PD69100_CMD_INDIVIDUAL_MASK_SET:
            msg.key = POE_PD69100_KEY_COMMAND;
            msg.echo = echo;
            msg.payload[0] = 0x07;
            msg.payload[1] = 0x56;

            va_start(args, cmd);
            data = va_arg(args, unsigned int);
            msg.payload[2] = data & 0xFF;
            data = va_arg(args, unsigned int);
            msg.payload[3] = (ENABLED == data) ? 1 : 0; 
            va_end(args);

            break;

        case POE_PD69100_CMD_TEMP_MATRIX_SET:
            msg.key = POE_PD69100_KEY_COMMAND;
            msg.echo = echo;
            msg.payload[0] = 0x05;
            msg.payload[1] = 0x43;

            va_start(args, cmd);
            data = va_arg(args, unsigned int);
            msg.payload[2] = data & 0xFF;
            data = va_arg(args, unsigned int);
            msg.payload[3] = data & 0xFF;
            va_end(args);

            msg.payload[4] = 0xFF;

            break;

        case POE_PD69100_CMD_GLOBAL_MATRIX_SET:
            msg.key = POE_PD69100_KEY_COMMAND;
            msg.echo = echo;
            msg.payload[0] = 0x07;
            msg.payload[1] = 0x43;

            break;

        case POE_PD69100_CMD_PM_METHOD_SET:
            msg.key = POE_PD69100_KEY_COMMAND;
            msg.echo = echo;
            msg.payload[0] = 0x07;
            msg.payload[1] = 0x0B;
            msg.payload[2] = 0x5F;

            va_start(args, cmd);
            data = va_arg(args, unsigned int);
            msg.payload[3] = data & 0xFF;
            data = va_arg(args, unsigned int);
            msg.payload[4] = data & 0xFF;
            data = va_arg(args, unsigned int);
            msg.payload[5] = data & 0xFF;
            va_end(args);

            break;

        case POE_PD69100_CMD_L2_PD_DATA_SET:
            msg.key = POE_PD69100_KEY_COMMAND;
            msg.echo = echo;
            msg.payload[0] = 0x05;
            msg.payload[1] = 0xA6;

            va_start(args, cmd);
            data = va_arg(args, unsigned int);
            msg.payload[2] = data & 0xFF;           /* CH number */
            data = va_arg(args, unsigned int);
            msg.payload[3] = data & 0xFF;           /* type */
            data = va_arg(args, unsigned int);
            msg.payload[4] = (data >> 8) & 0xFF;    /* PD request Power */
            msg.payload[5] = data & 0xFF;           /* PD request Power */
            data = va_arg(args, unsigned int);
            msg.payload[6] = (data >> 8) & 0xFF;    /* PSE Allocated Power */
            msg.payload[7] = data & 0xFF;           /* PSE Allocated Power */
            data = va_arg(args, unsigned int);
            msg.payload[5] = data & 0xFF;           /* Cable length */
            data = va_arg(args, unsigned int);
            msg.payload[9] = data & 0xFF;           /* Execute lldp */
            va_end(args);

            break;

        case POE_PD69100_CMD_TOTAL_POWER_SET:
            msg.key = POE_PD69100_KEY_COMMAND;
            msg.echo = echo;
            msg.payload[0] = 0x07;
            msg.payload[1] = 0x0B;
            msg.payload[2] = 0x57;
            msg.payload[3] = 0x00;

            va_start(args, cmd);
            data = va_arg(args, unsigned int);
            msg.payload[4] = (data >> 8) & 0xFF;    /* power limit */
            msg.payload[5] = data & 0xFF;
            va_end(args);

            msg.payload[6] = 0x02;                  /* Max. shutdown voltage */
            msg.payload[7] = 0x3A;
            msg.payload[8] = 0x01;                  /* Min. shutdown voltage */
            msg.payload[9] = 0xB8;
            msg.payload[10] = 0x01;                 /* Guard Band */

            break;

        case POE_PD69100_CMD_PORT_ADMIN_SET:
            msg.key = POE_PD69100_KEY_COMMAND;
            msg.echo = echo;
            msg.payload[0] = 0x05;
            msg.payload[1] = 0x0C;

            va_start(args, cmd);
            data = va_arg(args, unsigned int);
            msg.payload[2] = data & 0xFF;           /* port */
            data = va_arg(args, unsigned int);
            msg.payload[3] = (ENABLED == data) ? 1 : 0;   /* enable/disable */
            va_end(args);

            break;

        case POE_PD69100_CMD_PORT_PRIORITY_SET:
            msg.key = POE_PD69100_KEY_COMMAND;
            msg.echo = echo;
            msg.payload[0] = 0x05;
            msg.payload[1] = 0x0C;

            va_start(args, cmd);
            data = va_arg(args, unsigned int);
            msg.payload[2] = data & 0xFF;           /* port */
            data = va_arg(args, unsigned int);
            msg.payload[3] = data & 0xFF;          /* priority */
            va_end(args);

            break;

        case POE_PD69100_CMD_PORT_POWER_LIMIT_SET:
            msg.key = POE_PD69100_KEY_COMMAND;
            msg.echo = echo;
            msg.payload[0] = 0x05;
            msg.payload[1] = 0x0B;

            va_start(args, cmd);
            data = va_arg(args, unsigned int);
            msg.payload[2] = data & 0xFF;           /* port */
            data = va_arg(args, unsigned int);
            msg.payload[3] = (data >> 8) & 0xFF;    /* PPL */
            msg.payload[4] = data & 0xFF;           /* PPL */
            va_end(args);

            break;

        case POE_PD69100_CMD_RESTORE_FACTORY_DEFAULT_SET:
            msg.key = POE_PD69100_KEY_PROGRAM;
            msg.echo = echo;
            msg.payload[0] = 0x2D;

            break;

        case POE_PD69100_CMD_FW_DOWNLOAD_SET:
            is_delay = FALSE;
            msg.key = POE_PD69100_KEY_PROGRAM;
            msg.echo = echo;
            msg.payload[0] = 0xFF;
            msg.payload[1] = 0x99;
            msg.payload[2] = 0x15;
            msg.payload[3] = 0x16;
            msg.payload[4] = 0x16;
            msg.payload[5] = 0x99;
            break;

        default:
            return SYS_ERR_UNKNOWN;
            break;
    }

    /* checksum */
    SYS_ERR_CHK(poe_pd69100_message_checksum(&msg, &checksum1));
    memcpy(msg.checksum, &checksum1, sizeof(unsigned short));
    
    SYS_ERR_CHK(pd69100_message_write(&msg));

    memset(&msg, 0, sizeof(poe_pd69100_msg_t));

    /* delay ? */
    if (is_delay)
    {
        delay = POE_PD69100_CMD_DELAY / 10;
        while (delay)
        {
            retval = pd69100_message_read(&msg, echo);
            if (SYS_ERR_OK == retval)
                break;

            if (--delay)
                udelay(10 * 1000);
        }
    }
    else
    {
        SYS_ERR_CHK(pd69100_message_read(&msg, echo));
    }

    /* REPORT ? */
    if (POE_PD69100_KEY_REPORT != msg.key)
        return SYS_ERR_FAILED;

    /* checksum ? */
    memcpy(&checksum2, msg.checksum, sizeof(unsigned short));
    memset(msg.checksum, 0, sizeof(msg.checksum));
    SYS_ERR_CHK(poe_pd69100_message_checksum(&msg, &checksum1));
    if (checksum2 != checksum1)
        return SYS_ERR_FAILED;

    /* Success ? */
    if (0 != msg.payload[0] || 0 != msg.payload[1])
        return SYS_ERR_FAILED;

    return SYS_ERR_OK;
}

int32 poe_pd69100_firmware_get(void)
{
    int32 retval = SYS_ERR_OK;
    uint32 version = 0;
    sys_poe_msg_t msg;
    sys_poe_fw_t pPoeFirmware;

    pPoeFirmware.isFirmwareOk = FALSE;

    retval = poe_pd69100_cmd_get(POE_PD69100_CMD_SYSTEM_STATUS_GET, &msg);
    if (SYS_ERR_OK != retval)
        goto out;

    if (0 == (msg.payload[0] & (1 << 1)))
        pPoeFirmware.isFirmwareOk = TRUE;

    if (TRUE == pPoeFirmware.isFirmwareOk)
    {
        retval = poe_pd69100_cmd_get(POE_PD69100_CMD_FW_VERSION_GET, &msg);
        if (SYS_ERR_OK != retval)
        {
            pPoeFirmware.isFirmwareOk = FALSE;
            goto out;
        }

        version = ((msg.payload[3] << 8) | msg.payload[4]);

        pPoeFirmware.majorVer = version / 100;
        pPoeFirmware.minorVer = version % 100;
        printf("\n poe Firmware version %d.%d\n",pPoeFirmware.majorVer,pPoeFirmware.minorVer);
    }
    else
    	printf("\n poe need to download\n");
    return retval;
out:
		printf("\n poe get firmware version fail.\n");
    return retval;
}

int32 poe_pd69100_power_get(void)
{
    int32 retval = SYS_ERR_OK;
    sys_poe_msg_t msg;
		sys_poe_power_t pPower;

    retval = poe_pd69100_cmd_get(POE_PD69100_CMD_TOTAL_POWER_GET, &msg);

    if (SYS_ERR_OK == retval)
    {
        memset(&pPower, 0, sizeof(sys_poe_power_t));

        /* FIXME: maxPower from CFG */
        pPower.totalConsumed = msg.payload[0];
        pPower.totalConsumed <<= 8;
        pPower.totalConsumed |= msg.payload[1];
        pPower.totalConsumed *= 1000;
    
        pPower.totalCalculated = msg.payload[2];
        pPower.totalCalculated <<= 8;
        pPower.totalCalculated |= msg.payload[3];
        pPower.totalCalculated *= 1000;
        
        printf("\n poe Power Consumption %d, Calculated Power %d\n",
        pPower.totalConsumed,pPower.totalCalculated);
    }
    else
    	printf("\n poe fail to get total power.\n");

    return retval;
}

int poe_pd69100_init(void)
{
    int retval;
    poe_pd69100_msg_t msg;

//    sys_logic_port_t logic_port;
//    sys_physic_port_t physic_port;

		poe_uart_init(0);
    memset(&msg, 0, sizeof(poe_pd69100_msg_t));
    /* system status */
    retval = poe_pd69100_cmd_get(POE_PD69100_CMD_SYSTEM_STATUS_GET, &msg);
   
    if (SYS_ERR_OK == retval)
    {
        if (0 != (msg.payload[0] & (1 << 0)))
        {
        	  printf("\n POE report it get in troble.!\n");
            /* POE controller error */
            return SYS_ERR_FAILED;
        }
        else if (0 != (msg.payload[0] & (1 << 1)))
        {
            /* fw download required */
            printf("\n POE report it it need dowload firmware.!\n");
            //poe_pd69100_fw_download();
        }
    }
    else
    {
    		printf("\n POE Fail to get respone.!\n");
    		return SYS_ERR_FAILED;
        //poe_pd69100_fw_download();
    }

    /* enabled legacy mode */
    SYS_ERR_CHK(poe_pd69100_cmd_set(POE_PD69100_CMD_SYSTEM_MASK_SET, 0xFE));

#if 0
    /* enabled lldp */
    SYS_ERR_CHK(poe_pd69100_cmd_set(POE_PD69100_CMD_INDIVIDUAL_MASK_SET, 0x2E, DISABLED));
#endif
    /* enable dbg */
    SYS_ERR_CHK(poe_pd69100_cmd_set(POE_PD69100_CMD_INDIVIDUAL_MASK_SET, 0x29, ENABLED));
#if 0
    /* port mapping */
    FOR_EACH_NORMAL_LOGIC_PORT(logic_port)
    {
        SYS_ERR_CHK(sys_logic_to_physic_port(logic_port, &physic_port));
        SYS_ERR_CHK(poe_pd69100_cmd_set(POE_PD69100_CMD_TEMP_MATRIX_SET, logic_port, physic_port.port));
    }

    SYS_ERR_CHK(poe_pd69100_cmd_set(POE_PD69100_CMD_GLOBAL_MATRIX_SET));
#endif
#if 0
    /* each port */
    FOR_EACH_NORMAL_LOGIC_PORT(logic_port)
    {
        SYS_ERR_CHK(poe_pd69100_cmd_set(POE_PD69100_CMD_PORT_ADMIN_SET, logic_port, ENABLED));
        SYS_ERR_CHK(poe_pd69100_cmd_set(POE_PD69100_CMD_PORT_PRIORITY_SET, logic_port, 3));
        SYS_ERR_CHK(poe_pd69100_cmd_set(POE_PD69100_CMD_PORT_POWER_LIMIT_SET, logic_port, 31500));
    }
#endif

    poe_pd69100_enable(ENABLED);
		udelay(3000);
		poe_pd69100_firmware_get();
		poe_pd69100_power_get();
    return SYS_ERR_OK;
}
