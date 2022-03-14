#include <Lab_CA.h>

char rsa_encryptData[256] = {0}; // the encrypted temperature data
uint32_t rsa_encryptLen = 256; // the length of encrypted data

int Data_Read() {
    TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = SECURE_LAB_UUID_ID;
	uint32_t err_origin;

    /* Initialize a context connecting us to the TEE */
    res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS) {
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
    }

	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS) {
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x", res, err_origin);
    }
    
    /* Invoke the secure world function */
    memset(&op, 0, sizeof(op));
    op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT,
                                    TEEC_VALUE_OUTPUT,
                                    TEEC_NONE,
                                    TEEC_NONE);


    res = TEEC_InvokeCommand(&sess, CMD_PTA_DHT11_READ_DATA, &op,
                                &err_origin);
    if (res != TEEC_SUCCESS) {
        errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
    }

    printf("\n  start to receive dht11 data\n");
    printf("RH:%d.%d\n", op.params[0].value.a, op.params[0].value.b);
    printf("TMP:%d.%d\n", op.params[1].value.a, op.params[1].value.b);
    
    /*Close the session and destroy the context*/
    TEEC_CloseSession(&sess);
	TEEC_FinalizeContext(&ctx);
}

int Data_Encrypt() {
    TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = SECURE_LAB_UUID_ID;
	uint32_t err_origin;

    /* Initialize a context connecting us to the TEE */
    res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS) {
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
    }

	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS) {
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x", res, err_origin);
    }
    
    /* Invoke the secure world function */
    memset(&op, 0, sizeof(op));
    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT,
                                    TEEC_NONE,
                                    TEEC_NONE,
                                    TEEC_NONE);
    op.params[0].tmpref.buffer = rsa_encryptData;
    op.params[0].tmpref.size = rsa_encryptLen;
    
    res = TEEC_InvokeCommand(&sess, CMD_RSA_ENC_PKCS1_OPER, &op,
                                &err_origin);
    if (res != TEEC_SUCCESS) {
        errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
    }
    printf("The encrypted data is :\n");
    for (int i = 0; i < rsa_encryptLen; i++) {
        if (i % 16 == 0) {
            printf("\n");
        }
        printf("0x%02x, ", (rsa_encryptData[i] & 0x000000FF));
    }
    printf("\n");
    
    /*Close the session and destroy the context*/
    TEEC_CloseSession(&sess);
	TEEC_FinalizeContext(&ctx);
}

void Data_Send() {
    char systemCall[1660] = "mosquitto_pub -p 8883 -h 192.168.31.125 --cafile ca.crt -t t1 --debug -m  \""; 
    char tempbuf[6];
	for (int i = 0; i < rsa_encryptLen; i++) {
		sprintf(tempbuf, "0x%02x, ", rsa_encryptData[i]);
		strncat(systemCall, tempbuf, 6);
	}
    strncat(systemCall, "\"", 1);
    // invoke the system call
	system(systemCall);
}

int main(int argc, char *argv[])
{
    /*We will use DHT11 sensor to read temperature data*/
    /*The DHT11 driver is in secure world privileged mode*/
    if (0 == memcmp(argv[1], "1", 1)) {
        Data_Read();
    }

    /*The temperature data will be encrypted in secure world with RSA algorithm*/
    if (0 == memcmp(argv[1], "2", 1)) {
        Data_Encrypt();
    }

    /*The data will be sent to the MQTT broker*/
    if (0 == memcmp(argv[1], "3", 1)) {
        Data_Encrypt();
        Data_Send();
    }

    return 0;
}