#include <Lab_CA.h>

#define ENCRYPTED_DATA_LEN 256 // the encrypted data length
char encrypted_data[ENCRYPTED_DATA_LEN] = ""; // the encrypted data
#define DECRYPTED_DATA_LEN 256 // the decrypted data max length
char decrypted_data[DECRYPTED_DATA_LEN] = ""; // the decrypted data

/****************************************
* The received data format is hex.
* We need to convert the format to string
*****************************************/
void ByteOrNum(char c, int *temp) {
	if (c >= 'a' && c <= 'f') {
		*temp += 10 + c - 'a';
	} else if (c >= '0' && c <= '9') {
		*temp += c - '0';
	}
}
void hex2str(char* str) {
	int len = strlen(str);
	int j = 0;
	char tempChar[1];
	for (int i = 0; i < len; i+=6) {
		int temp =  0;
		ByteOrNum(str[i + 2], &temp);
		temp *= 16;
		ByteOrNum(str[i + 3], &temp);
		sprintf(tempChar, "%c", temp);
		encrypted_data[j++] = tempChar[0];
	}
}

/*
*  Decrypt received data
*/
void Data_Decrypt(char* received_data) {
    printf("The data received is below: %s\n", received_data);
    hex2str(received_data);

    TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = SECURE_LAB_SUB_UUID_ID;
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
    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
                                    TEEC_MEMREF_TEMP_OUTPUT,
                                    TEEC_NONE,
                                    TEEC_NONE);
    op.params[0].tmpref.buffer = encrypted_data;
    op.params[0].tmpref.size = ENCRYPTED_DATA_LEN;
    op.params[1].tmpref.buffer = decrypted_data;
    op.params[1].tmpref.size = DECRYPTED_DATA_LEN;
    
    res = TEEC_InvokeCommand(&sess, CMD_RSA_DEC_PKCS1_OPER, &op,
                                &err_origin);
    if (res != TEEC_SUCCESS) {
        errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
    }
    printf("The decrypt data is :\n%s\n", decrypted_data);
    
    return ;
}

int main(int argc, char* argv[])
{
    // Decrypt the received data
    Data_Decrypt(argv[1]);
    
    return 0;
}