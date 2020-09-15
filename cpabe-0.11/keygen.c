#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>
#include <time.h>

#include "bswabe.h"
#include "common.h"
#include "policy_lang.h"

#define DEBUG(arg) printf("file %s Line #%d\n", __FILE__, arg);


char *usage =
	"Usage: cpabe-keygen [OPTION ...] PUB_KEY MASTER_KEY ATTR [ATTR ...]\n"
	"\n"
	"Output will be written to the file\n"
	"\"priv_key\" unless the -o option is specified.\n"
	"\n"
	"Sample usage: keygendemo pub_key master_key 4 1 0 1 1\n"
	;

/*
	TODO ensure we don't give out the same attribute more than once (esp
	as different numerical values)
*/

char *pub_file = 0;
char *msk_file = 0;
char **attrs = 0;

char *out_file = "priv_key";

// gint comp_string(gconstpointer a, gconstpointer b)
// {
// 	return strcmp(a, b);
// }

int parse_args(int argc, char **argv)
{
	int i;
	GSList *alist;
	GSList *ap;
	int n;

	alist = 0;
	// printf("ARGC is %d", argc);
	for (i = 1; i < argc; i++){

		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
		{
			printf("%s", usage);
			exit(0);
		}
		else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
		{
			printf(CPABE_VERSION, "-keygen");
			exit(0);
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
			// printf("Setting public file\n");
			pub_file = argv[i];
		}
		else if (!msk_file)
		{
			// printf("Setting master file\n");
			msk_file = argv[i];
		}
		else
		{
			// printf("taking attributes at %d\n", i);
			parse_attribute(&alist, argv[i]);
		}
	}
	// printf("%s", alist);

	if (!pub_file || !msk_file || !alist)
		die(usage);

	//alist = g_slist_sort(alist, comp_string);
	n = g_slist_length(alist);
	// printf("Alist size is %d\n", n);
	// printf("%s\n", alist->data);
	// alist=alist->next;
	// printf("%s\n", alist->data);
	// alist=alist->next;
	// printf("%s\n", alist->data);
	attrs = malloc((n + 1) * sizeof(char *));

	// // ap = alist; ap = ap->next; ap = ap->next;
	// i = 0;
	// for (ap = alist; ap; ap = ap->next)
	// {
	// 	attrs[i] = ap->data;
	// 	printf("\nattrs[%d]= [%s] ", i, attrs[i]);
	// 	i++;
	// }
	ap = alist;
	// int i;
	// printf("i = %d n = %d\n", i, n);
	for( i=0;i<n;i++){
	
		// DEBUG(__LINE__);	
		attrs[i] = ap->data;
		// printf("\nattrs[%d]= [%s] \n", i, attrs[i]);
		ap = ap->next;
	}
		// printf("i = %d n = %d\n", i, n);
	// DEBUG(__LINE__);
	attrs[i] = 0;
	// DEBUG(__LINE__);
	return n;
}

int main(int argc, char **argv)
{
	bswabe_pub_t *pub;
	bswabe_msk_t *msk;
	bswabe_prv_t *prv;
	clock_t t1, t2;
	float diff;
	srand(time(NULL));
	int n = parse_args(argc, argv);
	t1 = clock();
	pub = bswabe_pub_unserialize_new(suck_file(pub_file), 1);
	msk = bswabe_msk_unserialize_new(suck_file(msk_file), 1);

	int attributes[n];
	for (int i = 0; i < n; i++)
		if (strcmp(attrs[i], "1") == 0)
			attributes[i] = 1;
		else
			attributes[i] = 0;

	bswabe_keygen(&prv, pub, msk, attributes);

	spit_file(out_file, bswabe_prv_serialize_new(prv), 1);
	t2 = clock();

	diff = ((double)(t2 - t1) / CLOCKS_PER_SEC);
	printf("\nTime taken in seconds=%f\n\n", diff);
	return 0;
}
