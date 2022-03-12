#include <Lab_CA.h>

int Data_Read() {
    TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = SECURE_LAB_UUID_ID;
	uint32_t err_origin;

    /* Initialize a context connecting us to the TEE */
    res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, err_origin);
    
    /* Invoke the secure world function */
    memset(&op, 0, sizeof(op));
    op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT,
                                    TEEC_VALUE_OUTPUT,
                                    TEEC_NONE,
                                    TEEC_NONE);


    res = TEEC_InvokeCommand(&sess, CMD_PTA_DHT11_READ_DATA, &op,
                                &err_origin);
    if (res != TEEC_SUCCESS)
        errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
                res, err_origin);

    printf("\n**************************\n start to receive dht11 data\n");
    printf("RH:%d.%d\n", op.params[0].value.a, op.params[0].value.b);
    printf("TMP:%d.%d\n", op.params[1].value.a, op.params[1].value.b);
    
    /*Close the session and destroy the context*/
    TEEC_CloseSession(&sess);
	TEEC_FinalizeContext(&ctx);
}

int main(int argc, char *argv[])
{
    /*We will use DHT11 sensor to read temperature data*/
    /*The DHT11 driver is in secure world privileged mode*/
    Data_Read();

    /*The temperature data will be encrypted in secure world with RSA algorithm*/
    //Data_Encrypt();

    /*The data will be sent to the MQTT broker*/
    //Data_Send();

    return 0;
}