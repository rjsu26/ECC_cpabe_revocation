#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <pbc.h>

#include "bswabe.h"
#include "private.h"

void serialize_string(GByteArray *b, mpz_t x)
{
	char *s;
	//mpz_t a;
	//mpz_init_set(a,x);
	s = malloc(sizeof(char) * 100);
	mpz_get_str(s, 10, x);
	printf("\ns is %s\n", s);
	printf("\n\n%d\n\n", strlen(s));
	g_byte_array_append(b, (unsigned char *)s, strlen(s) + 1);
}

char *
unserialize_string(GByteArray *b, int *offset, mpz_t x)
{
	GString *s;
	char *r;
	char *c;

	s = g_string_sized_new(64);
	while (1)
	{
		c = b->data[(*offset)++];
		if (c && c != EOF)
		{
			//printf("@@@%s\n",c);
			g_string_append_c(s, c);
		}
		else
			break;
	}

	r = s->str;
	g_string_free(s, 0);

	printf("unserialized string is %s\n", r);

	mpz_init_set_str(x, r, 10);

	return r;
}

GByteArray *
bswabe_pub_serialize(bswabe_pub_t *pub)
{

	GByteArray *b;
	int i; //,j,temp;
	b = g_byte_array_new();
	gmp_printf("value of n is %Zd", pub->n);
	uint64_t n = 3;
	printf("value of n in serialize is %d", n);
	printf("n serialized\n");
	serialize_string(b, pub->N);
	printf("N serialized");
	serialize_string(b, pub->Y);
	printf("Y serialized");
	serialize_string(b, pub->R);
	printf("R serialized");
	serialize_string(b, pub->D_u);
	printf("D_u serialized");
	mpz_t j;
	mpz_t val;
	mpz_init_set(val, pub->n);
	i = 0;
	mpz_init(j);
	serialize_string(b, val);
	while (mpz_cmp(j, pub->n) < 0)
	{ //

		serialize_string(b, pub->p_i[i]);
		mpz_add_ui(j, j, 1);
		i++;
	}
	return b;
}

bswabe_pub_t *
bswabe_pub_unserialize(GByteArray *b, int free)
{
	bswabe_pub_t *pub;
	int offset, i;

	pub = (bswabe_pub_t *)malloc(sizeof(bswabe_pub_t));
	offset = 0;

	int n = 3;
	printf("value of n is %d unserialized", n);
	//int n = mpz_get_ui(n);
	unserialize_string(b, &offset, pub->N);
	gmp_printf("value of N is %Zd", pub->N);
	unserialize_string(b, &offset, pub->Y);
	gmp_printf("value of Y is %Zd", pub->Y);
	unserialize_string(b, &offset, pub->R);
	gmp_printf("value of R is %Zd", pub->R);
	unserialize_string(b, &offset, pub->D_u);
	gmp_printf("value of D_u is %Zd", pub->D_u);
	mpz_t j;
	i = 0;
	mpz_init(j);
	unserialize_string(b, &offset, pub->n);
	while (mpz_cmp(j, pub->n) < 0)
	{
		unserialize_string(b, &offset, pub->p_i[i]);
		gmp_printf("value of p_i is %Zd\n", pub->p_i[i]);
		mpz_add_ui(j, j, 1);
		i++;
	}

	gmp_printf("value of n is %Zd\n", pub->n);

	if (free)
		g_byte_array_free(b, 1);

	return pub;
}

GByteArray *
bswabe_msk_serialize(bswabe_msk_t *msk, int n)
{
	GByteArray *b;
	bswabe_pub_t *pub;
	int i; //,j;
	b = g_byte_array_new();
	//int m = mpz_get_ui(pub ->n);
	serialize_string(b, msk->k);
	serialize_string(b, msk->x);
	serialize_string(b, msk->p);
	serialize_string(b, msk->q);
	mpz_t j;
	mpz_t val;
	mpz_init_set(val, msk->n);
	i = 0;
	mpz_init(j);
	serialize_string(b, msk->n);
	while (mpz_cmp(j, val) < 0)
	{ //
		serialize_string(b, msk->q_i[i]);
		mpz_add_ui(j, j, 1);
		i++;
	}

	return b;
}

bswabe_msk_t *
bswabe_msk_unserialize(GByteArray *b, int free)
{
	bswabe_msk_t *msk;
	int offset, i;

	msk = (bswabe_msk_t *)malloc(sizeof(bswabe_msk_t));

	offset = 0;

	// int m = 3;

	unserialize_string(b, &offset, msk->k); //
	unserialize_string(b, &offset, msk->x);

	gmp_printf("value of key is %Zd\n\n", msk->k);
	unserialize_string(b, &offset, msk->p);
	unserialize_string(b, &offset, msk->q);

	mpz_t j;
	i = 0;
	mpz_init(j);
	unserialize_string(b, &offset, msk->n);
	while (mpz_cmp(j, msk->n) < 0)
	{
		unserialize_string(b, &offset, msk->q_i[i]);
		mpz_add_ui(j, j, 1);
		i++;
	}

	if (free)
		g_byte_array_free(b, 1);

	return msk;
}

GByteArray *
bswabe_prv_serialize(bswabe_prv_t *prv)
{
	GByteArray *b;
	int i;

	b = g_byte_array_new();

	serialize_string(b, prv->k1);
	serialize_string(b, prv->k2);
	serialize_string(b, prv->e_a);

	return b;
}

bswabe_prv_t *
bswabe_prv_unserialize(bswabe_pub_t *pub, GByteArray *b, int free)
{
	bswabe_prv_t *prv;
	int i;
	//int len;
	int offset;

	prv = (bswabe_prv_t *)malloc(sizeof(bswabe_prv_t));
	offset = 0;

	unserialize_string(b, &offset, prv->k1);
	unserialize_string(b, &offset, prv->k2);

	unserialize_string(b, &offset, prv->e_a);

	if (free)
		g_byte_array_free(b, 1);

	return prv;
}

GByteArray *
bswabe_cph_serialize(bswabe_cph_t *cph)
{
	GByteArray *b;
	int i;

	b = g_byte_array_new();
	serialize_string(b, cph->Y_m);
	serialize_string(b, cph->R_m);
	serialize_string(b, cph->C_m);
	serialize_string(b, cph->C_sigma);
	serialize_string(b, cph->S_m);
	serialize_string(b, cph->e_p);

	return b;
}

bswabe_cph_t *
bswabe_cph_unserialize(bswabe_pub_t *pub, GByteArray *b, int free)
{
	bswabe_cph_t *cph;
	int offset, i;

	cph = (bswabe_cph_t *)malloc(sizeof(bswabe_cph_t));
	offset = 0;

	unserialize_string(b, &offset, cph->Y_m);
	unserialize_string(b, &offset, cph->R_m);
	unserialize_string(b, &offset, cph->C_m);
	unserialize_string(b, &offset, cph->C_sigma);
	unserialize_string(b, &offset, cph->S_m);
	unserialize_string(b, &offset, cph->e_p);
	if (free)
		g_byte_array_free(b, 1);

	return cph;
}

void bswabe_pub_free(bswabe_pub_t *pub)
{
	int i, j;
	int n = mpz_get_ui(pub->n);
	for (i = 0; i < n; i++)
	{
		mpz_clear(pub->p_i[i]);
	}
	mpz_clear(pub->n);
	mpz_clear(pub->N);
	mpz_clear(pub->Y);
	mpz_clear(pub->R);
	mpz_clear(pub->D_u);

	free(pub);
}

void bswabe_msk_free(bswabe_msk_t *msk)
{
	int i, j;
	int n = mpz_get_ui(msk->n);

	for (i = 0; i < n; i++)
	{
		mpz_clear(msk->q_i[i]);
	}
	mpz_clear(msk->n);
	mpz_clear(msk->k);
	mpz_clear(msk->x);
	mpz_clear(msk->p);
	mpz_clear(msk->q);
	free(msk);
}

void bswabe_prv_free(bswabe_prv_t *prv)
{
	int i, j;

	mpz_clear(prv->k1);
	mpz_clear(prv->k2);
	mpz_clear(prv->e_a);

	free(prv);
}

void bswabe_policy_free(bswabe_policy_t *p)
{
	int i;

	if (p->attr)
	{
		free(p->attr);
		element_clear(p->c);
		element_clear(p->cp);
	}

	for (i = 0; i < p->children->len; i++)
		bswabe_policy_free(g_ptr_array_index(p->children, i));

	g_ptr_array_free(p->children, 1);

	free(p);
}

void bswabe_cph_free(bswabe_cph_t *cph)
{
	mpz_clear(cph->Y_m);
	mpz_clear(cph->R_m);
	mpz_clear(cph->C_m);
	mpz_clear(cph->C_sigma);
	mpz_clear(cph->S_m);
	mpz_clear(cph->e_p);
	free(cph);
}
