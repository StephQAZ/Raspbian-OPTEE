#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <Lab_TA.h>

/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void)
{
	DMSG("has been called");

	return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
	DMSG("has been called");
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA. In this function you will normally do the global initialization for the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param __maybe_unused params[4],
		void __maybe_unused **sess_ctx)
{
	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;

	/* If return value != TEE_SUCCESS the session will not be created. */
	return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
	IMSG("Goodbye!\n");
}

static TEE_Result dht11_read_data(uint32_t param_types,
	TEE_Param params[4])
{
	uint32_t exp_param_types = TEE_PARAM_TYPES (TEE_PARAM_TYPE_VALUE_OUTPUT,
						   TEE_PARAM_TYPE_VALUE_OUTPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	TEE_Result res = TEE_ERROR_GENERIC;
    TEE_UUID uuid = PTA_RPI3DHT11_UUID;
    TEE_TASessionHandle session = TEE_HANDLE_NULL;
    uint32_t ret_orig = 0;
    TEE_Param pta_params[4]; // parameters sent to pseudo TA
    uint32_t pta_param_types = 0; // parameters variable
    
    // 1. open pseudo TA session
    res = TEE_OpenTASession(&uuid, TEE_TIMEOUT_INFINITE,
                            0, pta_params, &session, &ret_orig);
    if (res != TEE_SUCCESS) {
        EMSG("Open pta session failed, res=0x%08x, origin=0x%08x", res, ret_orig);
        return res;
    }
    // 2. invoke pseudo TA functions
    pta_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
                                    TEE_PARAM_TYPE_VALUE_OUTPUT,
                                    TEE_PARAM_TYPE_NONE,
                                    TEE_PARAM_TYPE_NONE);
    res = TEE_InvokeTACommand(session, TEE_TIMEOUT_INFINITE, DHT11_READ_DATA,
                                pta_param_types, pta_params, NULL);
    if (res != TEE_SUCCESS) {
        EMSG("Invoke pta failed, res=0x%08x, origin=0x%08x", res, ret_orig);
        TEE_CloseTASession(session);
        session = TEE_HANDLE_NULL;
        return res;
    }
    params[0].value.a = pta_params[0].value.a;
    params[0].value.b = pta_params[0].value.b;
    params[1].value.a = pta_params[1].value.a;
    params[1].value.b = pta_params[1].value.b;
    // 3. close pseudo TA session
    TEE_CloseTASession(session);
    session = TEE_HANDLE_NULL;
	return TEE_SUCCESS;
}

/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the paramters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sess_ctx,
			uint32_t cmd_id,
			uint32_t param_types, TEE_Param params[4])
{
	(void)&sess_ctx; /* Unused parameter */

	switch (cmd_id) {
	case CMD_PTA_DHT11_READ_DATA:
		return dht11_read_data(param_types, params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
