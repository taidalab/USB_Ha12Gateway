/***************************************************************************//**
 * @file ecode.h
 * @brief Energy Aware drivers error code definitions.
 * @version 4.2.3
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#ifndef __SILICON_LABS_ECODE_H__
#define __SILICON_LABS_ECODE_H__

/***************************************************************************//**
 * @addtogroup EM_Drivers
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief Typedef for API function errorcode return values.
 *
 * @details
 *        Bit 24-31:  sub-system, for example emLib, eaDrivers,... @n
 *        Bit 16-23:  module, for example UART, LCD,... @n
 *        Bit 0-15:   error code, specific error code
 ******************************************************************************/
typedef uint32_t Ecode_t;

#define ECODE_EMDRV_BASE  ( 0xF0000000 )  ///< Base value for all EMDRV errorcodes.

#define ECODE_OK          ( 0 )           ///< Generic success return value.

#define ECODE_EMDRV_RTCDRV_BASE     ( ECODE_EMDRV_BASE | 0x00001000 ) ///< Base value for RTCDRV error codes.
#define ECODE_EMDRV_SPIDRV_BASE     ( ECODE_EMDRV_BASE | 0x00002000 ) ///< Base value for SPIDRV error codes.
#define ECODE_EMDRV_NVM_BASE        ( ECODE_EMDRV_BASE | 0x00003000 ) ///< Base value for NVM error codes.
#define ECODE_EMDRV_USTIMER_BASE    ( ECODE_EMDRV_BASE | 0x00004000 ) ///< Base value for USTIMER error codes.
#define ECODE_EMDRV_AESDRV_BASE     ( ECODE_EMDRV_BASE | 0x00005000 ) ///< Base value for AESDRV error codes.
#define ECODE_EMDRV_ECC_BASE        ( ECODE_EMDRV_BASE | 0x00006000 ) ///< Base value for ECC error codes.
#define ECODE_EMDRV_UARTDRV_BASE    ( ECODE_EMDRV_BASE | 0x00007000 ) ///< Base value for UARTDRV error codes.
#define ECODE_EMDRV_DMADRV_BASE     ( ECODE_EMDRV_BASE | 0x00008000 ) ///< Base value for DMADRV error codes.
#define ECODE_EMDRV_EZRADIODRV_BASE ( ECODE_EMDRV_BASE | 0x00009000 ) ///< Base value for EZRADIODRV error codes.
#define ECODE_EMDRV_SHADRV_BASE     ( ECODE_EMDRV_BASE | 0x0000A000 ) ///< Base value for SHADRV error codes.
#define ECODE_EMDRV_RSADRV_BASE     ( ECODE_EMDRV_BASE | 0x0000B000 ) ///< Base value for RSADRV error codes.
#define ECODE_EMDRV_CRYPTO_BASE     ( ECODE_EMDRV_BASE | 0x0000C000 ) ///< Base value for CRYPTO error codes.
#define ECODE_EMDRV_TEMPDRV_BASE    ( ECODE_EMDRV_BASE | 0x0000D000 ) ///< Base value for TEMPDRV error codes.

/** @} (end addtogroup EM_Drivers) */

#endif // __SILICON_LABS_ECODE_H__
