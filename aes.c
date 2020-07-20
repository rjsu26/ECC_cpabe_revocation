// #include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>
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
    // DEBUG(__LINE__);
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    // DEBUG(__LINE__);
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
    // DEBUG(__LINE__);
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    // DEBUG(__LINE__);
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    // DEBUG(__LINE__);
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

 char *in = "trial.txt";
char * out = "trial.txt.cpabe";


char* encryption_testing(){
    printf("Inside encryption function...\n");
    char *in_file = in;
    char * out_file = out;

    // Generate a random key
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *temp = BN_new();

    printf("Generating key...\n");
    BN_rand(temp,256,-1,1); 
    char* key =BN_bn2dec(temp); 
	// printf("%s\n",key);
    // DEBUG(__LINE__);
    printf("Generating IV...\n");
    BN_rand(temp,128,-1,1); 
    char* iv = BN_bn2dec(temp);
	// printf("%s\n",iv);

// Read file 
	long long file_len;
	GByteArray *plt;
	GByteArray *cph_buf;
    printf("Sucking file...\n");
    plt = suck_file(in_file);
	file_len = plt->len;
    printf("Read file of length: %lld\n", file_len);

    unsigned char *plaintext =(unsigned char *)plt->data;
    // printf("Plaintext is : %s\n", plaintext);
    long long  output_size = strlen ((char *)plaintext) + (16 - (strlen ((char *)plaintext) % 16));
    printf("Output size of length: %lld\n", output_size);
    unsigned char ciphertext[ output_size];

    // Encrypt the text
    int ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv, ciphertext);

    // Spit the ciphertext
    // printf("Ciphertext is: \n%s\n",ciphertext);
    cph_buf->data = ciphertext;
    cph_buf->len = ciphertext_len;
    printf("Ciphertext length: %d\n", ciphertext_len);
    printf("Spitting file...\n");
    spit_file(out_file, ciphertext, ciphertext_len);

    // Generate the concatenated key
    char *M = (char*)malloc( strlen(key)+1+strlen(iv)+1); // key + " " + IV + "\0" 
    strcpy(M,key);
    strcat(M, " ");
    strcat(M, iv);
    printf("Generating the concatenated key..\n");
    // printf("%s\n", M);

    return M;

}

void decryption_testing(char* kv){
    char *in_file = out;
    char * out_file = in;

    // Extract key and iv from kv(concatenated key and iv)
    printf("Extracting the key and IV..\n");
    char* delim=" ";
    char *ptr = strtok(kv, delim);
    char* key=ptr;
    // printf("%s\n", key);
    ptr = strtok(NULL, delim);
    char* iv = ptr;
    // printf("%s\n", iv);

    // Read the ciphertext file
    int file_len;
	GByteArray *plt;
	GByteArray *cph_buf;
    printf("Sucking file...\n");
    plt = suck_file(in_file);
	file_len = plt->len;
    printf("Read file of length: %d\n", file_len);

    unsigned char *ciphertext =(unsigned char *)plt->data;

    // Decrypting the text   
    unsigned char decryptedtext[ strlen ((char *)ciphertext)];
    unsigned char decryptedtext[ strlen ((char *)ciphertext)];
    printf("Output size of length: %ld\n", strlen(decryptedtext));

    printf("Decrypting file..\n"); 
    int decryptedtext_len = decrypt(ciphertext,  strlen (decryptedtext), key, iv, decryptedtext);
    printf("Plaintext length: %d\n", decryptedtext_len);

    // Spit file
    printf("Spitting file..\n"); 
    spit_file(out_file, decryptedtext, decryptedtext_len);


}

int main(void)
{
	// GByteArray *aes_buf;

    char *M=encryption_testing();
    decryption_testing(M);
    // int decryptedtext_len, ciphertext_len =  strlen ((char *)ciphertext);
    // BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

    /* Decrypt the ciphertext */
    // DEBUG(__LINE__);
    // decryptedtext[decryptedtext_len] = '\0';
    // printf("Decrypted text is:\n");
    // printf("%s\n", decryptedtext);
    /* Add a NULL terminator. We are expecting printable text */

    // DEBUG(__LINE__);
    /* Show the decrypted text */

    return 0;
}