#define BUFFER_SIZE 512

char clearText[BUFFER_SIZE] = {0};
char cipherText[BUFFER_SIZE] = {0};

static int cra_aes_cbc_init(struct crypto_skcipher *tfm);

static int cra_aes_cbc_setkey (struct crypto_skcipher *tfm, const u8 *key,
	              unsigned int keylen);

static int cra_aes_cbc_encrypt(struct skcipher_request *req);

static int cra_aes_cbc_decrypt(struct skcipher_request *req);

static void cra_aes_cbc_exit(struct crypto_skcipher *tfm);

