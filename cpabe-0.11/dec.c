#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>
#include <time.h>

#include "bswabe.h"
#include "common.h"

char *usage =
	"Usage: cpabe-dec [OPTION ...] PUB_KEY PRIV_KEY FILE\n"
	"\n"
	"Decrypt FILE using private key PRIV_KEY and assuming public key\n"
	"PUB_KEY. If the name of FILE is X.cpabe, the decrypted file will\n"
	"be written as X and FILE will be removed. Otherwise the file will be\n"
	"decrypted in place. Use of the -o option overrides this\n"
	"behavior.\n"
	"\n"
	"Mandatory arguments to long options are mandatory for short options too.\n\n"
	" -h, --help               print this message\n\n"
	" -v, --version            print version information\n\n"
	" -k, --keep-input-file    don't delete original file\n\n"
	" -o, --output FILE        write output to FILE\n\n"
	" -d, --deterministic      use deterministic \"random\" numbers\n"
	"                          (only for debugging)\n\n"
	/* " -s, --no-opt-sat         pick an arbitrary way of satisfying the policy\n" */
	/* "                          (only for performance comparison)\n\n" */
	/* " -n, --naive-dec          use slower decryption algorithm\n" */
	/* "                          (only for performance comparison)\n\n" */
	/* " -f, --flatten            use slightly different decryption algorithm\n" */
	/* "                          (may result in higher or lower performance)\n\n" */
	/* " -r, --report-ops         report numbers of group operations\n" */
	/* "                          (only for performance evaluation)\n\n" */
	"";

/* enum { */
/* 	DEC_NAIVE, */
/* 	DEC_FLATTEN, */
/* 	DEC_MERGE, */
/* } dec_strategy = DEC_MERGE;		 */

char *pub_file = 0;
char *prv_file = 0;
char *in_file = 0;
char *out_file = 0;
/* int   no_opt_sat = 0; */
/* int   report_ops = 0; */
int keep = 0;

/* int num_pairings = 0; */
/* int num_exps     = 0; */
/* int num_muls     = 0; */

void parse_args(int argc, char **argv)
{
	int i;

	for (i = 1; i < argc; i++)
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
		{
			printf("%s", usage);
			exit(0);
		}
		else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
		{
			printf(CPABE_VERSION, "-dec");
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
			printf("##%s\n", argv[i]);
			pub_file = argv[i];
		}
		else if (!prv_file)
		{
			//printf("%s",argv[i]);
			prv_file = argv[i];
		}
		else if (!in_file)
		{
			in_file = argv[i];
		}
		else
			die(usage);

	if (!pub_file || !prv_file || !in_file)
		die(usage);

	if (!out_file)
	{
		if (strlen(in_file) > 6 &&
			!strcmp(in_file + strlen(in_file) - 6, ".cpabe"))
			out_file = g_strndup(in_file, strlen(in_file) - 6);
		else
			out_file = strdup(in_file);
	}

	if (keep && !strcmp(in_file, out_file))
		die("cannot keep input file when decrypting file in place (try -o)\n");
}

int main(int argc, char **argv)
{
	bswabe_pub_t *pub;
	bswabe_prv_t *prv;
	int file_len, temp = 0;

	GByteArray *aes_buf;
	GByteArray *plt;
	GByteArray *cph_buf;
	bswabe_cph_t *cph;
	// element_t m1;
	char* M;
	clock_t t1, t2;
	float diff;

	t1 = clock();
	srand(time(NULL));

	parse_args(argc, argv);
	printf("\nBefore pub unserialize");
	pub = bswabe_pub_unserialize_new(suck_file(pub_file), 1);
	//gmp_printf("\nAfter PUB unserialize %Zd #####", pub->n);

	prv = bswabe_prv_unserialize_new( suck_file(prv_file), 1);

	// Read cph and aes buffer from cpabe file in order. CHANGE FUNCTION ACCORDINGLY
	read_cpabe_file(in_file, &cph_buf, &file_len, &aes_buf);
	// Use spit_file again to just replace the whole file with just aes_buf, so that aes_cbs_encrypt function can work upon it directly.
	cph = bswabe_cph_unserialize(pub, cph_buf, 1);

	// Obtain decrypted value of AES key and iv
	if (!bswabe_dec(pub, prv, cph, M))
	{
		t2 = clock();
		diff = ((double)(t2 - t1) / CLOCKS_PER_SEC);
		printf("\nTime taken in seconds in=%f\n", diff);
		printf("Unable to Decrypt. Exiting !!!\n");
		die("%s", bswabe_error());
	}

	t2 = clock();
	diff = ((double)(t2 - t1) / CLOCKS_PER_SEC);
	printf("\nTime taken in seconds af=%f", diff);
	// printf("\nAfter bswabe_dec\n");
	// bswabe_cph_free(cph);
	// printf("\ncph\n");
	// element_printf("\n Value of m in dec =%B \n", m1);


	// Extracting key and iv from char* M
	char* delim=" ";
    char *ptr = strtok(M, delim);
    char* KEY=ptr;
    ptr = strtok(NULL, delim);
    char* IV = ptr;

    /* Open the encrypted file for reading in binary ("rb" mode) */
    FILE *f_input = fopen(in_file, "rb");
	    if (!f_input) {
        /* Unable to open file for reading */
        fprintf(stderr, "ERROR: fopen error: %s\n", strerror(errno));
        return errno;
    }

    /* Open and truncate file to zero length or create decrypted file for writing */
    f_dec = fopen(out_file, "wb");
    if (!f_dec) {
        /* Unable to open file for writing */
        fprintf(stderr, "ERROR: fopen error: %s\n", strerror(errno));
        return errno;
    }


    /* Decrypt the given file */
	aes_cbc_256(f_input, f_enc, encrypt,KEY, IV);
    /* Close the open file descriptors */
    fclose(f_input);
    fclose(f_dec);

	// plt = aes_128_cbc_decrypt(aes_buf, m1);

	// g_byte_array_set_size(plt, file_len);
	// printf("\n size \n");

	g_byte_array_free(aes_buf, 1);
	// printf("\n free \n");

	// spit_file(out_file, plt, 1);
	t2 = clock();
	diff = ((double)(t2 - t1) / CLOCKS_PER_SEC);
	printf("\nTime taken in seconds=%f\n", diff);

	if (!keep)
		unlink(in_file);
	
	return 0;
}
