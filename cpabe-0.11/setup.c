#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>
#include <time.h>

#include "bswabe.h"
#include "common.h"

#define DEBUG(arg) printf("file %s Line #%d\n", __FILE__, arg);

char *usage =
	"Usage: cpabe-setup [OPTION ...]\n"
	"\n"
	"Generate a public key, and a master secret key\n"
	"for use with cpabe-keygen, cpabe-enc, and cpabe-dec.\n"
	"\n"
	"Output will be written to the files \"pub_key\" and \"master_key\"\n"
	"unless the --output-public-key or --output-master-key options are\n"
	"used.\n"
	"\n"
	" -h, --help                    print this message\n\n"
	" -p, --output-public-key FILE  write public key to FILE\n\n"
	" -m, --output-master-key FILE  write master secret key to FILE\n\n"
	" -d, --deterministic           use deterministic \"random\" numbers\n"
	"                               (only for debugging)\n\n"
	"";

char *pub_file = "pub_key";
char *msk_file = "master_key";

void parse_args(int argc, char **argv)
{
	int i;

	for (i = 1; i < argc; i++)

		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
		{
			printf("%s", usage);
			exit(0);
		}
		else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--output-public-key"))
		{
			if (++i >= argc)
				die(usage);
			else
				pub_file = argv[i];
		}
		else if (!strcmp(argv[i], "-m") || !strcmp(argv[i], "--output-master-key"))
		{
			if (++i >= argc)
				die(usage);
			else
				msk_file = argv[i];
		}
		else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--deterministic"))
		{
			pbc_random_set_deterministic(0);
		}
		else
			die(usage);
}

int main(int argc, char **argv)
{
	// printf("\nHello\n");
	bswabe_pub_t *mpk;
	bswabe_msk_t *msk;
	int n;
	clock_t t1, t2;
	float diff;

	t1 = clock();
	srand(time(NULL));

	parse_args(argc, argv);
	// DEBUG(__LINE__);
	//printf("\n before call \n");
	printf("\nEnter no of attributes in system : ");
	scanf("%d", &n); printf("\n");
	if (n<=0){
		printf("Must have atleast 1 attribute..\n");
		return 0;
	}
	// printf("n: %d\n",n);
	// printf("Line 83 setup");
	// printf("Setup line: %d",__LINE__);
	DEBUG(__LINE__);
	bswabe_setup(&mpk, &msk, n);
	//printf("\n after call \n");
	//spit_file(pub_file,bswabe_pub_serialize(mpk),1);
	DEBUG(__LINE__);
	spit_file(pub_file, bswabe_pub_serialize_new(mpk), 1);
	spit_file(msk_file, bswabe_msk_serialize_new(msk), 1);

	t2 = clock();

	//diff=((float)(t2 - t1) / 1000000.0F ) * 1000;
	diff = ((double)(t2 - t1) / CLOCKS_PER_SEC);
	printf("\nTime taken in seconds=%f", diff);
	return 0;
}
