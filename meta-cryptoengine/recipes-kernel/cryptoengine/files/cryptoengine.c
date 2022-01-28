// SPDX-License-Identifier: GPL-2.0-or-later
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/crypto.h>
#include <linux/scatterlist.h>
#include <crypto/internal/skcipher.h>
#include <crypto/aes.h>
#include "cryptoengine.h"

// #include "./build/tmp/sysroots-components/imx8qmmek/optee-os/usr/include/optee/export-user_ta_arm64/include/limits.h"
/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>
// #include "/home/dalil/yoctoProject/imx-yocto-bsp/build/tmp/work/imx8qmmek-poky-linux/cryptoengine/1.0-r0/tee_client_api.h"
/* For the UUID (found in the TA's h-file(s)) */
#include "crypto_engine_ta.h"

TEEC_Result res;

TEEC_Context ctx;
TEEC_Session sess;
TEEC_Operation op;

TEEC_UUID uuid = TA_CRYPTO_ENGINE_UUID;

uint32_t err_origin;
uint32_t cmd;

static struct skcipher_alg youssef_aes_cbc_alg = {

    .base = {

        .cra_name = "cbc(aes)",

        .cra_driver_name = "youssef-cbc-aes",

        .cra_type = CRYPTO_ALG_TYPE_SKCIPHER,

        .cra_priority = 300,

        .cra_module = THIS_MODULE,

    },

    .min_keysize = AES_MIN_KEY_SIZE,

    .max_keysize = AES_MAX_KEY_SIZE,

    .ivsize = AES_BLOCK_SIZE,

    .chunksize = AES_BLOCK_SIZE,

    .walksize = AES_BLOCK_SIZE,

    .init = cra_aes_cbc_init,
    
    .setkey = cra_aes_cbc_setkey,

    .encrypt = cra_aes_cbc_encrypt,

    .decrypt = cra_aes_cbc_decrypt,

    .exit = cra_aes_cbc_exit,
};

static int cra_aes_cbc_init(struct crypto_skcipher *tfm){

    pr_info("CRA_AES_CBC init crypto engine \n");

    memset(&ctx, 0, sizeof(ctx));

    /* Initialize a context connecting us to the TEE */
	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

    memset(&sess, 0, sizeof(sess));

    /*
	 * Open a session to the "crypto engine" TA, the TA will print "hello
	 * world!" in the log when the session is created.
	 */
    res = TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
    if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, err_origin);

    return 0;
}

static int cra_aes_cbc_setkey (struct crypto_skcipher *tfm, const u8 *key,
	              unsigned int keylen){

    pr_info("CRA_AES_CBC setkey crypto engine \n");

    cmd = TA_CRYPTO_ENGINE_CMD_SET_KEY_VALUE;

    memset(&op, 0, sizeof(op));

    /*
	 * Prepare the argument. Pass a value in the first parameter,
	 * the remaining three parameters are unused.
	 */
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);

    op.params[0].tmpref.buffer = key;
    op.params[0].tmpref.size = keylen;

    res = TEEC_InvokeCommand(&sess, TA_CRYPTO_ENGINE_CMD_SET_KEY_VALUE, &op, &err_origin);

    if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);

    return 0;
}


static int cra_aes_cbc_encrypt(struct skcipher_request *req){

    pr_info("CRA_AES_CBC encrypt crypto engine \n");

    cmd = TA_CRYPTO_ENGINE_CMD_ENCRYPT_VALUE;

    memset(&op, 0, sizeof(op));

    /*
	 * Prepare the argument. Pass a value in the first parameter,
	 * the remaining three parameters are unused.
	 */
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_NONE, TEEC_NONE);

    op.params[0].tmpref.buffer = clearText;
    op.params[0].tmpref.size = BUFFER_SIZE;

    op.params[1].tmpref.buffer = cipherText;
    op.params[1].tmpref.size = BUFFER_SIZE;

    res = TEEC_InvokeCommand(&sess, cmd, &op, &err_origin);

    if(res != TEEC_SUCCESS)
        errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);

    return 0;
}

static int cra_aes_cbc_decrypt(struct skcipher_request *req){

    pr_info("CRA_AES_CBC decrypt crypto engine \n");

    cmd = TA_CRYPTO_ENGINE_CMD_DECRYPT_VALUE;

    memset(&op, 0, sizeof(op));

    /*
	 * Prepare the argument. Pass a value in the first parameter,
	 * the remaining three parameters are unused.
	 */
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_NONE, TEEC_NONE);

    op.params[0].tmpref.buffer = cipherText;
    op.params[0].tmpref.size = BUFFER_SIZE;

    op.params[1].tmpref.buffer = clearText;
    op.params[1].tmpref.size = BUFFER_SIZE;

    res = TEEC_InvokeCommand(&sess, cmd, &op, &err_origin);

    if(res != TEEC_SUCCESS)
        errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);

    return 0;
}

static void cra_aes_cbc_exit(struct crypto_skcipher *tfm){

    pr_info("CRA_AES_CBC exit crypto engine \n");

    res =  TEEC_CloseSession(&sess);

    if(res != TEEC_SUCCESS)
        errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res);

    TEEC_FinalizeContext(&ctx);
    
}


static int __init cryptoalg_init(void){

    int ret;

    ret = crypto_register_skcipher(&youssef_aes_cbc_alg);

    if (ret < 0){
        pr_err("youssef_aes_cbc driver not registred : error %d \n", ret);
    }

    else pr_info("youssef_aes_cbc driver well registred");

    return ret;
}

static void __exit cryptoalg_exit(void){
    
    crypto_unregister_skcipher(&youssef_aes_cbc_alg);

    pr_info("youssef_aes_cbc driver well unregistred");
    
}

module_init(cryptoalg_init);
module_exit(cryptoalg_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Crypto hardware engine");