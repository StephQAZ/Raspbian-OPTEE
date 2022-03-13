#ifndef TA_SECURE_LAB_H
#define TA_SECURE_LAB_H

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <tee_api_defines.h>
#include <trace.h>
#include <string.h>

#define SECURE_LAB_UUID_ID \
	{ 0xec0b0d54, 0xd7a4, 0x450b, \
		{ 0x9a, 0x45, 0x4b, 0x49, 0xf3, 0x9f, 0xf2, 0xf0} }

/* The function IDs implemented in this TA */
#define CMD_PTA_DHT11_READ_DATA		5
#define CMD_RSA_ENC_PKCS1_OPER      6

/* The function IDs implemented in the pseudo TA*/
#define GPIO_ON					0
#define GPIO_OFF				1
#define TEST_PSEUDO_TA 			2
#define DHT11_READ_DATA			3
#define PTA_RPI3DHT11_UUID \
	{ 0xbd867e69, 0x58fb, 0x4675, \
		{ 0x83, 0x26, 0x5a, 0x68, 0xa3, 0xbf, 0xcc, 0x7f}}

/*
*************************************************************************
*                  RSA algorithm
*************************************************************************
*/
// typedef unsigned char  UINT8;    /**< Typedef for 8bits unsigned integer  */
// typedef unsigned short UINT16;   /**< Typedef for 16bits unsigned integer */
// typedef uint32_t       UINT32;   /**< Typedef for 32bits unsigned integer */
// typedef signed char    INT8;     /**< Typedef for 8bits signed integer    */
// typedef signed short   INT16;    /**< Typedef for 16bits signed integer   */
// typedef signed int     INT32;    /**< Typedef for 32bits signed integer   */
// typedef char           CHAR;     /**< Typedef for char                    */
#define   FAIL     		-1            /* Return value when operation fail */
#define   OK        	0            /* Return value when operation OK */

typedef struct _RsaKey
{
    char* Ndata;
    char* Ddata;
    char* Edata;
    uint32_t Nlen;
    uint32_t Dlen;
    uint32_t Elen;
    uint32_t padding;
}RsaKey;

#endif
