#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <openssl/bn.h>
#include <glib.h>
#include <sys/stat.h>
#define DEBUG(arg) printf("\nfile %s Line #%d\n", __FILE__, arg);


FILE *
fopen_read_or_die(char *file)
{
    FILE *f;

    if (!(f = fopen(file, "r")))
        exit(1);

    return f;
}

FILE *fopen_write_or_die(char *file)
{
    FILE *f;

    if (!(f = fopen(file, "w")))
        exit(1);
    return f;
}

GByteArray *suck_file(char *file)
{
    FILE *f;
    GByteArray *a;
    struct stat s;

    a = g_byte_array_new();
    stat(file, &s);
    g_byte_array_set_size(a, s.st_size);

    f = fopen_read_or_die(file);
    fread(a->data, 1, s.st_size, f);
    fclose(f);

    return a;
}

void spit_file(char *file, unsigned char* data,int len)
{
    FILE *f;

    f = fopen_write_or_die(file);
    fwrite(data, 1, len, f);
    fclose(f);

    // if (free)
    //     g_byte_array_free(b, 1);
}
void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    DEBUG(__LINE__);
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    DEBUG(__LINE__);
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;
    printf("Inside decrypt..\n");
    // printf("\n");
    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    DEBUG(__LINE__);
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    DEBUG(__LINE__);
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    DEBUG(__LINE__);
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

int main(void)
{
    /*
     * Set up the key and iv. Do I need to say to not hard code these in a
     * real application? :-)
     */
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *temp = BN_new();
    BN_rand(temp,256,-1,1); 

	unsigned char M[500];
	strcpy(M, (char *)BN_bn2dec(temp));
    DEBUG(__LINE__);
	printf("%s\n",M);
	strcpy(M, (char *)"\n");
	printf("%s\n",M);
    DEBUG(__LINE__);
    BN_rand(temp,128,-1,1); 
	strcpy(M, (char *)BN_bn2dec(temp));
	printf("%s\n",M);
    DEBUG(__LINE__);
    return 0;
    /* A 256 bit key */
    // unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
	int file_len;
	GByteArray *plt;
	GByteArray *cph_buf;
	GByteArray *aes_buf;

    char *in_file = "trial.txt.cpabe";
    char * out_file = "trial2.txt";
    
    plt = suck_file(in_file);
	file_len = plt->len;

    // BN_CTX *ctx = BN_CTX_new();
    // BIGNUM *x = BN_new();
    // BN_rand(x,256,-1,1);

    unsigned char *key = (unsigned char *)"68262122174558485127461951687933255009356209528694341651094844657405511223941";
    // unsigned char *key = (unsigned char *)BN_bn2dec(x);
    // printf("\nKey: %s\n", key);
    /* A 128 bit IV */

    // BN_rand(x,128,-1,1);
    // unsigned char *iv = (unsigned char *)BN_bn2dec(x);
    unsigned char *iv = (unsigned char *)"35274296234310999958121204970206483393";
    // printf("\nIV is : %s\n", iv);

   unsigned char *ciphertext =
        (unsigned char *)plt->data;
//    unsigned char *plaintext =
//         (unsigned char *)plt->data;

    // printf("Plaintext is : %s\n", plaintext);
    /*
     * Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, depending on the
     * algorithm and mode.
     */
    // int output_size = strlen ((char *)plaintext) + (16 - (strlen ((char *)plaintext) % 16));
    // unsigned char ciphertext[ output_size];

    /* Buffer for the decrypted text */

    int decryptedtext_len, ciphertext_len =  strlen ((char *)ciphertext);

    /* Encrypt the plaintext */
    DEBUG(__LINE__);
    printf("length of ciphertext is : %ld\n", strlen ((char *)ciphertext));
    // printf("Ciphertext is : %s\n", ciphertext);
    // printf("Key is : %s\n", key);
    // printf("IV is : %s\n", iv);

    // DEBUG(__LINE__);
    // ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv, ciphertext);
    DEBUG(__LINE__);
    // printf("Ciphertext is: \n%s\n",ciphertext);
    // cph_buf->data = ciphertext;
    // cph_buf->len = ciphertext_len;
    // spit_file(out_file, ciphertext, ciphertext_len);
    /* Do something useful with the ciphertext here */
    // printf("Ciphertext is:\n");

    // printf("Size : %ld\n", sizeof(ciphertext));
    // printf("%s", ciphertext);
    // BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);
    // DEBUG(__LINE__);
    unsigned char decryptedtext[ strlen ((char *)ciphertext)];
    /* Decrypt the ciphertext */
    decryptedtext_len = decrypt(ciphertext,  strlen ((char *)ciphertext), key, iv, decryptedtext);
    DEBUG(__LINE__);
    spit_file(out_file, decryptedtext, decryptedtext_len);
    decryptedtext[decryptedtext_len] = '\0';
    printf("Decrypted text is:\n");
    printf("%s\n", decryptedtext);
    return 0;
    /* Add a NULL terminator. We are expecting printable text */

    DEBUG(__LINE__);
    /* Show the decrypted text */

    return 0;
}