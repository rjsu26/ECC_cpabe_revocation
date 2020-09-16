#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <pbc.h>

#include "bswabe.h"
#include "private.h"

#define DEBUG(arg) printf("file %s Line #%d\n", __FILE__, arg);

/*just for reference
char *BN_bn2hex(const BIGNUM *a);
char *BN_bn2dec(const BIGNUM *a);
int BN_hex2bn(BIGNUM **a, const char *str);
int BN_dec2bn(BIGNUM **a, const char *str);
*/
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

// NEW
//serialize for bignum ans EC_POINT ============================================
void serialize_string_bn(GByteArray *b, BIGNUM *x)
{
    char *s;
    s = malloc(sizeof(char) * 100);
    s = BN_bn2hex(x); //char *BN_bn2hex(const BIGNUM *a);
    // printf("\ns is %s\n", s);
    // printf("\n\n%ld\n\n", strlen(s));
    g_byte_array_append(b, (unsigned char *)s, strlen(s) + 1);
}
// NEW
void serialize_string_point(GByteArray *b, EC_POINT *x, EC_GROUP *curve, BN_CTX* ctx)
{
    char *s;
    s = malloc(sizeof(char) * 100);
	DEBUG(__LINE__);
	// BIGNUM *x1 = BN_new();
    // BIGNUM *y1 = BN_new();
	// DEBUG(__LINE__);
    // EC_POINT_get_affine_coordinates_GFp(curve,x,x1,y1,ctx);
	// DEBUG(__LINE__);
    // BN_print_fp(stdout, x1);
    // putc('\n', stdout);
    // BN_print_fp(stdout, y1);
    // putc('\n', stdout);

	DEBUG(__LINE__);
    s = EC_POINT_point2hex(curve,x,POINT_CONVERSION_COMPRESSED, ctx); 
    // printf("\ns is %s\n", s);
    // printf("\n\n%ld\n\n", strlen(s));
	DEBUG(__LINE__);
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

	// printf("unserialized string is %s\n", r);

	mpz_init_set_str(x, r, 10);

	return r;
}

// NEW
char *unserialize_string_new(GByteArray *b, int *offset){
	GString *s;
    char *r, *c;
    s = g_string_sized_new(64);
    while (1)
    {
        c = b->data[(*offset)++];
        if (c && c != EOF)
            g_string_append_c(s, c);
        else
            break;
    }

    r = s->str;
	// DEBUG(__LINE__);
	// printf("%s\n", r);
    g_string_free(s, 0);

    return r;
}

//================================================================

// NEW
GByteArray *
bswabe_pub_serialize_new(bswabe_pub_t *pub)
{
	GByteArray *b;
    b = g_byte_array_new();
	
	// serializing n
	char *char_arr;
    char_arr=malloc(sizeof(char)*100);
	sprintf(char_arr, "%d", pub->n);
	g_byte_array_append(b, (unsigned char *)char_arr, strlen(char_arr)+1);

	// serializing p, a, b, order, G_x, G_y
	serialize_string_bn(b, pub->p);
	serialize_string_bn(b, pub->a);
	serialize_string_bn(b, pub->b);
	serialize_string_bn(b, pub->order);
	serialize_string_bn(b, pub->G_x);
	serialize_string_bn(b, pub->G_y);

	// serializing EC_POINT P_i[], U_i[] and V_i[]
	EC_GROUP *curve;
    curve = create_curve(pub->a,pub->b,pub->p,pub->order,pub->G_x,pub->G_y);
    BN_CTX *ctx = BN_CTX_new();

	for(int i=0;i<pub->n;i++)
		serialize_string_point(b, pub->P_i[i],curve,ctx);
	for(int i=0;i<pub->n;i++)
		serialize_string_point(b, pub->U_i[i],curve,ctx);
	for(int i=0;i<pub->n;i++)
		serialize_string_point(b, pub->V_i[i],curve,ctx);
	
	return b;
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

// NEW
bswabe_pub_t *bswabe_pub_unserialize_new(GByteArray *b, int free){
	BN_CTX *ctx;
    if (NULL == (ctx = BN_CTX_new()))
        printf("error\n");
	
	bswabe_pub_t *pub = (bswabe_pub_t*)malloc(sizeof(bswabe_pub_t));
	int offset=0;
	char* char_arr;
	char_arr = unserialize_string_new(b,&offset); // for n
	sscanf(char_arr, "%d", &(pub->n));
	// printf("pub->n is %d\n", pub->n);
	// DEBUG(__LINE__);

	BIGNUM *temp;
    temp = BN_new();

	char_arr = unserialize_string_new(b,&offset); // for a
	BN_hex2bn(&temp,char_arr);
	// printf("%s\n", char_arr);
	pub->p = BN_dup(temp);

	char_arr = unserialize_string_new(b,&offset); // for a
	BN_hex2bn(&temp,char_arr);
	// printf("%s\n", char_arr);
	pub->a = BN_dup(temp);
	// BN_copy(pub->a, temp);
	// DEBUG(__LINE__);

	char_arr = unserialize_string_new(b,&offset); // for b
	BN_hex2bn(&temp, char_arr);
	// printf("%s\n", char_arr);
	pub->b = BN_dup(temp);
	// BN_copy(pub->b, temp);

	char_arr = unserialize_string_new(b,&offset); // order
	BN_hex2bn(&temp, char_arr);
	// printf("\n%s\n", char_arr);
	// DEBUG(__LINE__);
	// BN_print_fp(stdout, temp);
	// printf("\n");
	pub->order = BN_dup(temp);

	char_arr = unserialize_string_new(b,&offset); // G_x
	BN_hex2bn(&temp, char_arr);
	// printf("\n%s\n", char_arr);
	pub->G_x = BN_dup(temp);

	char_arr = unserialize_string_new(b,&offset); // G_y
	BN_hex2bn(&temp, char_arr);
	pub->G_y = BN_dup(temp);


	EC_GROUP *curve;
    curve = create_curve(pub->a,pub->b,pub->p,pub->order,pub->G_x,pub->G_y);
	// EC_POINT* temp_point;
	// temp_point = EC_POINT_new(curve);

	for(int i=0;i<pub->n;i++){ // for P_i[ ]
		char_arr = unserialize_string_new(b,&offset); 
    	pub->P_i[i] = EC_POINT_hex2point(curve, char_arr, pub->P_i[i], ctx);
	}

	for(int i=0;i<pub->n;i++){ // for U_i[i]
		char_arr = unserialize_string_new(b,&offset); 
    	pub->U_i[i] = EC_POINT_hex2point(curve, char_arr, pub->U_i[i], ctx);

	}
	for(int i=0;i<pub->n;i++){  // for V_i[i]
		char_arr = unserialize_string_new(b,&offset); 
    	pub->V_i[i]=EC_POINT_hex2point(curve, char_arr, pub->V_i[i], ctx);
	}
	
	// BN_CTX_free(ctx);
	if (free)
		g_byte_array_free(b, 1);
	
	return pub;

}

// NEW
GByteArray *bswabe_msk_serialize_new(bswabe_msk_t *msk)
{
	GByteArray *b;
	b = g_byte_array_new();
	// serialize alpha, k1, k2
	serialize_string_bn(b, msk->alpha);
	serialize_string_bn(b, msk->k1);
	serialize_string_bn(b, msk->k2);
	return b;
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

// NEW
bswabe_msk_t *bswabe_msk_unserialize_new(GByteArray *b, int free){

	bswabe_msk_t *msk= (bswabe_msk_t *)malloc(sizeof(bswabe_msk_t));
	int offset=0 ;
	char* char_arr;
	
	// for alpha
	char_arr = unserialize_string_new(b,&offset); 
	BN_hex2bn(&(msk->alpha), char_arr);
	// for k1
	char_arr = unserialize_string_new(b,&offset); 
	BN_hex2bn(&(msk->k1), char_arr);
	// for k2
	char_arr = unserialize_string_new(b,&offset); 
	BN_hex2bn(&(msk->k2), char_arr);

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

// NEW
GByteArray *
bswabe_prv_serialize_new(bswabe_prv_t *prv)
{
	GByteArray *b;
	int i;

	b = g_byte_array_new();

	serialize_string_bn(b, prv->u1);
	serialize_string_bn(b, prv->u2);

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

// NEW
bswabe_prv_t *bswabe_prv_unserialize_new(GByteArray *b, int free){
	bswabe_prv_t *prv;
	int offset=0;
	prv = (bswabe_prv_t *)malloc(sizeof(bswabe_prv_t));
	char* char_arr;
	
	// for u1
	char_arr = unserialize_string_new(b,&offset); 
	BN_hex2bn(&(prv->u1), char_arr);
	// for u2
	char_arr = unserialize_string_new(b,&offset); 
	BN_hex2bn(&(prv->u2), char_arr);

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

// NEW
GByteArray *
bswabe_cph_serialize_new(bswabe_cph_t *cph, int n, bswabe_pub_t *pub){
	GByteArray *b;
    b = g_byte_array_new();
	char *char_arr;
	char_arr = malloc(sizeof(char)*100);
	DEBUG(__LINE__);
	// serializing policy[]
	for(int i=0;i<n;i++){
		sprintf(char_arr, "%d", cph->Policy[i]);
		g_byte_array_append(b, (unsigned char *)char_arr, strlen(char_arr)+1);
	}
	DEBUG(__LINE__);
	int ones = 0;                        // counting set bits in policy
    for(int i=0;i<n;i++){
        if(cph->Policy[i]==1)
            ++ones;
    }

	DEBUG(__LINE__);
	// serialising P_m_i
	BN_CTX *ctx = BN_CTX_new();
	EC_GROUP *curve;
    curve = create_curve(pub->a,pub->b,pub->p,pub->order,pub->G_x,pub->G_y);

	// for(int i=0;i<n;i++)
	// 	printf("%d ", cph->Policy[i]);
	// printf("%s \n%s\n", cph->C_sigma_m, cph->C_m);
	BIGNUM *x1 = BN_new();
	BIGNUM *y1 = BN_new();
	EC_POINT_get_affine_coordinates(curve,cph->P_m_i[0],x1,y1,ctx);

	BN_print_fp(stdout, x1);
	putc('\n', stdout);
	BN_print_fp(stdout, y1);
	putc('\n', stdout);
	DEBUG(__LINE__);
	for(int i=0; i<=n-ones; i++){
		// DEBUG(__LINE__);
		EC_POINT_get_affine_coordinates(curve,cph->P_m_i[i],x1,y1,ctx);
		BN_print_fp(stdout, x1);
		putc('\n', stdout);
		BN_print_fp(stdout, y1);
		putc('\n', stdout);
		// serialize_string_point(b, cph->P_m_i[i],curve,ctx);
	}
	DEBUG(__LINE__);

	// serialising K_1m and K_2m
	serialize_string_point(b, cph->K_1m,curve,ctx);
	serialize_string_point(b, cph->K_2m,curve,ctx);

	// serialise C_sigma_m and C_m
	g_byte_array_append(b, cph->C_sigma_m, strlen(cph->C_sigma_m)+1);
	g_byte_array_append(b, cph->C_m, strlen(cph->C_m)+1);

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

// NEW 
bswabe_cph_t* bswabe_cph_unserialize_new( bswabe_pub_t* pub, GByteArray* b, int free ){
	BN_CTX *ctx = BN_CTX_new();
	bswabe_cph_t *cph = (bswabe_cph_t*)malloc(sizeof(bswabe_cph_t));
	int offset=0, ones=0;
	char* char_arr;
	for(int i=0;i<pub->n;i++){
		char_arr = unserialize_string_new(b,&offset); // for n
		sscanf(char_arr, "%d", &(cph->Policy[i]));
		if(cph->Policy[i]==1) 
			ones++;
	}

	EC_GROUP *curve;
    curve = create_curve(pub->a,pub->b,pub->p,pub->order,pub->G_x,pub->G_y);

	// for P_m_i
	for(int i=0;i<=pub->n-ones;i++){
		char_arr = unserialize_string_new(b,&offset); 
    	cph->P_m_i[i] = EC_POINT_hex2point(curve, char_arr,cph->P_m_i[i], ctx);
	}

	// for K_1m
		char_arr = unserialize_string_new(b,&offset); 
    	cph->K_1m = EC_POINT_hex2point(curve, char_arr,cph->K_1m, ctx);

	// for K_2m
		char_arr = unserialize_string_new(b,&offset); 
    	cph->K_2m = EC_POINT_hex2point(curve, char_arr,cph->K_2m, ctx);

	if(free)
		g_byte_array_free(b,1);

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
