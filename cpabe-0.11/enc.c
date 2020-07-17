#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <openssl/bn.h>

#include "bswabe.h"
#include "common.h"
#include "policy_lang.h"

char *usage =
	"Usage: cpabe-enc [OPTION ...] PUB_KEY FILE [POLICY]\n"
	"\n"
	"Encrypt FILE under the decryption policy POLICY using public key\n"
	"PUB_KEY. The encrypted file will be written to FILE.cpabe unless\n"
	"the -o option is used. The original file will be removed. If POLICY\n"
	"is not specified, the policy will be read from stdin.\n"
	"\n"
	"Mandatory arguments to long options are mandatory for short options too.\n\n"
	" -h, --help               print this message\n\n"
	" -v, --version            print version information\n\n"
	" -k, --keep-input-file    don't delete original file\n\n"
	" -o, --output FILE        write resulting key to FILE\n\n"
	" -d, --deterministic      use deterministic \"random\" numbers\n"
	"                          (only for debugging)\n\n"
	"";

char *pub_file = 0;
char *msk_file = "master_key";
char *in_file = 0;
char *out_file = 0;
int keep = 0;
char **attrs = 0;

// char **policy = 0;
// char *verification_file = "verification_key";

int parse_args(int argc, char **argv)
{
	int i, j;
	int n, flag = 0;
	printf("\n Entry in parse_args ");
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
			printf("\nIn parse_args %d th arg pub_file", i);
		}
		else if (!in_file)
		{
			in_file = argv[i];
			flag = i + 1;
			printf("\nIn parse_args %d th arg in_file and flag=%d", i, flag);
		}
		else
		{
			printf("\nIn parse_args %d th arg %s", i, argv[i]);
			//policy = parse_policy_lang(argv[i]);
		}
	//else
	//	flag=0;
	//die(usage);

	if (!pub_file || !in_file)
		die(usage);

	if (!out_file)
		out_file = g_strdup_printf("%s.cpabe", in_file);

	//if( !policy )
	//	policy = parse_policy_lang(suck_stdin());
	/*n = g_slist_length(alist);
	printf("\nn=%d",n);*/
	n = argc - 3;
	printf("\nn=%d", n);
	attrs = malloc((n + 1) * sizeof(char *));
	i = 0;
	for (j = flag; j < argc; j++)
	{
		attrs[i] = argv[j];
		printf("\nattrs[%d]= [%s] ", i, attrs[i]);
		i++;
	}
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
	GByteArray *plt;
	GByteArray *cph_buf;
	GByteArray *aes_buf;
	element_t m;
	clock_t t1, t2;
	float diff;

	srand(time(NULL));

	int n = parse_args(argc, argv);
	t1 = clock();
	pub = bswabe_pub_unserialize_new(suck_file(pub_file), 1);
	// msk = bswabe_msk_unserialize_new(suck_file(msk_file), 1);
	
	int attributes[n];
	for (int i = 0; i < n; i++)
		if (strcmp(attrs[i], "1") == 0)
			attributes[i] = 1;
		else
			attributes[i] = 0;

	BN_CTX *ctx = BN_CTX_new();
    BIGNUM *key = BN_new();
    BN_rand(key,256,-1,1); 

	unsigned char *M;
	strcpy(M, (char *)BN_bn2dec(key));
	strcpy(M, (char *)'\n');
    BN_rand(key,128,-1,1); 
	strcpy(M, (char *)BN_bn2dec(key));
	
	if (!(cph = bswabe_enc(pub, msk, m, attributes)))
		die("%s", bswabe_error());
		
	cph_buf = bswabe_cph_serialize(cph);
	//bswabe_cph_free(cph);
	// printf("\n after cph_serialize");

	plt = suck_file(in_file);
	file_len = plt->len;
	aes_buf = aes_128_cbc_encrypt(plt, m);
	g_byte_array_free(plt, 1);
	// element_clear(m);

	write_cpabe_file(out_file, cph_buf, file_len, aes_buf);

	g_byte_array_free(cph_buf, 1);
	g_byte_array_free(aes_buf, 1);
	t2 = clock();
	diff = ((double)(t2 - t1) / CLOCKS_PER_SEC);
	printf("\nTime taken in seconds=%f", diff);

	if (!keep)
		unlink(in_file);

	return 0;
}
