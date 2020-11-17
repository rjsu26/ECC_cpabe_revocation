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

	srand(time(NULL));

	parse_args(argc, argv);
	// DEBUG(__LINE__);
	//printf("\n before call \n");
	printf("\nEnter no of attributes in system(1-100) : ");
	scanf("%d", &n);
	//  printf("\n");
	if (n<=0 || n>100){
		printf("Number of attributes incorrect. Retry..\n");
		return 0;
	}
	t1 = clock();
	bswabe_setup(&mpk, &msk, n);
	spit_file(pub_file, bswabe_pub_serialize_new(mpk), 1);
	spit_file(msk_file, bswabe_msk_serialize_new(msk), 1);

	t2 = clock();
	diff = ((double)(t2 - t1) / CLOCKS_PER_SEC);
	printf("\nTime taken = %f seconds \n\n", diff);
	return 0;
}
