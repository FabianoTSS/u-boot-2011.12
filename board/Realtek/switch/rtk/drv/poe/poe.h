/*
 * Copyright(c) Realtek Semiconductor Corporation, 2013
 * All rights reserved.
 *
 * Purpose : Related definition of the POE driver for RTK command.
 *
 * Feature : POE driver for RTK command
 *
 */

#ifndef	__POE_UART_H__
#define	__POE_UART_H__

#define BCM59111_UART_DEV               1
#define BCM59111_UART_BAUDRATE          19200

#define SYS_ERR_UNKNOWN -5
#define SYS_ERR_FAILED -1                             /* General Error */
#define SYS_ERR_OK  0                                  /* OK */

#define SYS_PARAM_CHK(expr, errCode)                        \
do {                                                        \
    if ((int)(expr)) {                                    \
        return errCode;                                     \
    }                                                       \
} while (0)

#define SYS_ERR_CHK(op)                                     \
do {                                                        \
    int errCode;                                          \
    if ((errCode = (op)) != SYS_ERR_OK)                    \
        return errCode;                                     \
} while(0)

#define SYS_ERR_CONTINUE(op)                                \
    if ((op) != SYS_ERR_OK) continue;
#endif

