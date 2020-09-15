#include <string.h>
#include <unistd.h>
#include <glib.h>

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <pbc.h>
#include <pbc_random.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#include "bswabe.h"
#include "common.h"
#include "policy_lang.h"

#define DEBUG(arg) printf("file %s Line #%d\n", __FILE__, arg);


char *usage =
	"Usage: cpabe-enc [OPTION ...] PUB_KEY FILE [POLICY]\n"
	"\n"
	"Encrypt FILE under the decryption policy POLICY using public key\n"
	"PUB_KEY. The encrypted file will be written to FILE.cpabe unless\n"
	"the -o option is used. The original file will be removed. If POLICY\n"
	"is not specified, the policy will be read from stdin.\n"
	"\n";

char *pub_file = 0;
char *msk_file = "master_key";
char *in_file = 0;
char *out_file ;
int keep = 0;
char **attrs = 0;

// char **policy = 0;
// char *verification_file = "verification_key";

int parse_args(int argc, char **argv)
{
	int i, j;
	int n, flag = 0;
	// printf("\n Entry in parse_args ");
	for (i = 1; i < argc; i++)
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
		{
			printf("%s", usage);
			exit(0);
		}
		else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
		{
			printf(CPABE_VERSION, "-enc");
			exit(0);
		}
		else if (!strcmp(argv[i], "-k") || !strcmp(argv[i], "--keep-input-file"))
		{
			keep = 1;
		}
		else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output"))
		{
			if (++i >= argc)
				die(usage);
			else
				out_file = argv[i];
		}
		else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--deterministic"))
		{
			pbc_random_set_deterministic(0);
		}
		else if (!pub_file)
		{
			pub_file = argv[i];
			// printf("\nIn parse_args %d th arg pub_file", i);
		}
		else if (!in_file)
		{
			in_file = argv[i];
			printf("Infile is : %s\n", in_file);
			flag = i + 1;
			// printf("\nIn parse_args %d th arg in_file and flag=%d", i, flag);
		}
		else
		{
			// printf("\nIn parse_args %d th arg %s", i, argv[i]);
			//policy = parse_policy_lang(argv[i]);
		}
	//else
	//	flag=0;
	//die(usage);

	if (!pub_file || !in_file)
		die(usage);

	if (!out_file){
		// out_file = g_strdup_printf("%s.cpabe", in_file);
		out_file = malloc(100);
		strcpy(out_file, in_file);
		// out_filein_file;
		strcat(out_file, ".cpabe");
		printf("outfile set as : %s", out_file); 
	}

	//if( !policy )
	//	policy = parse_policy_lang(suck_stdin());
	/*n = g_slist_length(alist);
	printf("\nn=%d",n);*/
	n = argc - 3;
	printf("\nn=%d\n", n);
	attrs = malloc((n + 1) * sizeof(char *));
	i = 0;
	for (j = flag; j < argc; j++)
	{
		attrs[i] = argv[j];
		// printf("attrs[%d]= [%s] \n", i, attrs[i]);
		i++;
	}
	// DEBUG(__LINE__);
	attrs[i] = 0;
	return n;
	//policy = attrs;
}

int main(int argc, char **argv)
{
	bswabe_pub_t *pub;
	bswabe_cph_t *cph;
	bswabe_msk_t *msk;
	int file_len;
	// GByteArray *plt;
	GByteArray *cph_buf;
	GByteArray *aes_buf;
	// element_t m;
	clock_t t1, t2;
	float diff;

	srand(time(NULL));
	int n = parse_args(argc, argv);
	t1 = clock();
	// DEBUG(__LINE__);
	pub = bswabe_pub_unserialize_new(suck_file(pub_file), 1);
	// msk = bswabe_msk_unserialize_new(suck_file(msk_file), 1);

	int attributes[n];
	for (int i = 0; i < n; i++)
		if (strcmp(attrs[i], "1") == 0)
			attributes[i] = 1;
		else
			attributes[i] = 0;

/* Print the saved attributes */	
// 	for(int i=0;i<n;i++)
	// 	printf("%d  ", attributes[i]);
	// printf("\n");

	// Do AES encryption of input file

	/* Key to use for encrpytion and decryption */
    unsigned char key[32];
    /* Initialization Vector */
    unsigned char iv[16];
    /* Generate cryptographically strong pseudo-random bytes for key and IV */
    if (!RAND_bytes(key, sizeof(key)) || !RAND_bytes(iv, sizeof(iv))) {
        /* OpenSSL reports a failure, act accordingly */
        fprintf(stderr, "ERROR: RAND_bytes error: %s\n", strerror(errno));
        return errno;
    }
	// unsigned char *KEY = key;
    // unsigned char *IV = iv;
    unsigned int encrypt = 1;
	
	// printf("key = %s\nIV = %s\n", key, iv);
	/* Open the input file for reading in binary ("rb" mode) */
    FILE *f_input = fopen(in_file, "rb");
	// printf("Infile is : %s\n", in_file);

    if (!f_input) {
        /* Unable to open file for reading */
        fprintf(stderr, "1ERROR: fopen error: %s\n", strerror(errno));
        return errno;
    }
	// char ch;
	// while((ch = fgetc(f_input)) != EOF)
    //   printf("%c", ch);

    /* Open and truncate file to zero length or create ciphertext file for writing */
	FILE *f_out = fopen(out_file, "wb");
    if (!f_out) {
        /* Unable to open file for writing */
        fprintf(stderr, "2ERROR: fopen error: %s\n", strerror(errno));
        return errno;
    }
	// AES encrypt the in_file and save it to out_file. The out_file will be sucked again for cpabe work. 
	aes_cbc_256(f_input, f_out, encrypt,key, iv); 
	fclose(f_out);
	f_out = fopen(out_file, "rb");
	// printf("Outfile content is \n");

    if (!f_out) {
        /* Unable to open file for reading */
        fprintf(stderr, "1ERROR: fopen error: %s\n", strerror(errno));
        return errno;
    }
	// for(int i=0;i<32;i++){
    //     printf("%d ", key[i] );
    // }
    // printf("\n");
    BIGNUM *Mon= BN_bin2bn(key, sizeof(key), NULL);
    // BN_print_fp(stdout, Mon);printf("\n");
	unsigned char *to = malloc((BN_num_bytes(Mon) +1)*sizeof(char));
    // BN_bn2bin(Mon, to);
    // for(int i=0;i<32;i++){
    //     printf("%d ", to[i] );
    // }
    // printf("\n");
	// DEBUG(__LINE__);

	// sleep(5);
	// generate the concatenated string of key and IV
	// char *M = (char*)malloc( strlen(KEY)+1+strlen(IV)); // KEY + " " + IV
	// strcpy(M,key);
    // strcat(M, " ");
    // strcat(M, IV);
	// printf("combined key = %s\n", M);
	// encrypt the message M
	if (!(cph = bswabe_enc_new(pub, msk, Mon, attributes)))
		die("%s", bswabe_error());
	
	// serialize the ciphertext parameters
	DEBUG(__LINE__);
	cph_buf = bswabe_cph_serialize(cph);
	bswabe_cph_free(cph);

	aes_buf = suck_file(out_file); // re-reading the aes-encrypted file
	file_len = aes_buf->len;
	// aes_buf = aes_128_cbc_encrypt(plt, m);
	// g_byte_array_free(plt, 1);
	// element_clear(m);
	
	// Write the cph_buf, a new line and then aes_buffer to the out_file
	// CHANGE THIS FUNCTION ACCORDINGLY 
	write_cpabe_file(out_file, cph_buf, file_len, aes_buf);

	g_byte_array_free(cph_buf, 1);
	g_byte_array_free(aes_buf, 1);
	t2 = clock();
	diff = ((double)(t2 - t1) / CLOCKS_PER_SEC);
	printf("\nTime taken in seconds=%f\n", diff);

	if (!keep)
		unlink(in_file);

	return 0;
}
