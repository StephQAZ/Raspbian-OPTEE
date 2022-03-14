#ifndef LAB_CA_HEADER_H
#define LAB_CA_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "tee_client_api.h"

//ec0b0d54-d7a4-450b-9a45-4b49f39ff2f0
#define SECURE_LAB_UUID_ID \
    {0xec0b0d54, 0xd7a4, 0x450b, \
        { 0x9a, 0x45, 0x4b, 0x49, 0xf3, 0x9f, 0xf2, 0xf0}}

/* Define the type of variable */
typedef unsigned char  UINT8;    /*< Typedef for 8bits unsigned integer  */
typedef unsigned short UINT16;   /*< Typedef for 16bits unsigned integer */
typedef unsigned int   UINT32;   /*< Typedef for 32bits unsigned integer */
typedef signed char    INT8;     /*< Typedef for 8bits signed integer    */
typedef signed short   INT16;    /*< Typedef for 16bits signed integer   */
typedef signed int     INT32;    /*< Typedef for 32bits signed integer   */
typedef char           CHAR;     /*< Typedef for char                    */

#define CMD_PTA_DHT11_READ_DATA           5U     /*< Command ID of using pseudo TA to read DHT11 sensor data */
#define CMD_RSA_ENC_PKCS1_OPER            6U     /*< Command ID of using RSA algorithm to signa data */
#define CMD_RSA_DEC_PKCS1_OPER            7U


#define   FAIL     -1            /* Return value when operation fail */
#define   OK        0            /* Return value when operation OK */


/* RSA key type(1024,2048) */
typedef enum
{
    EN_KEY_1024 = 1,
    EN_KEY_2048,
    EN_KEY_INVALID
}EN_RSA_KEY_TYPE;

typedef struct RsaCaPara_s
{
    CHAR* m_pInput;
    CHAR* m_pOutput;
    UINT32 m_InputLen;
    UINT32 m_OutputLen;
    UINT32 cmdId;
    EN_RSA_KEY_TYPE Rsa_Elect;
}RsaCaPara;

extern int g_CryptoVerifyCa_Rsa(RsaCaPara rsaPara);

#endif
