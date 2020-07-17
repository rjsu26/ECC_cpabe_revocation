/*
	Include glib.h, pbc.h, and bswabe.h before including this file.
*/
#include <openssl/bn.h>
#include <openssl/ec.h>


struct bswabe_pub_s //  USE IT AS MPK
{
	// mpz_t n;
	mpz_t N;
	mpz_t Y;
	mpz_t R;
	mpz_t p_i[100];
	mpz_t D_u; // CAPS OFF

	// ==============Our ECC stuff====================

	int n; // number of attributes n, also the size of P_i having actual attributes. Need not be of mpz_t type
	BIGNUM *p; // prime number p
	BIGNUM *a; // a of Elliptic curve equation 
	BIGNUM *b; // b of EC equation 
	BIGNUM *order; // order of the EC curve
	BIGNUM *G_x; // generator point x
	BIGNUM *G_y; // generator point y
	EC_POINT *P_i[100], *U_i[100], *V_i[100];
};

struct bswabe_msk_s // use it as MSK
{

	mpz_t k;
	mpz_t x;
	mpz_t p;
	mpz_t q;
	mpz_t n;
	mpz_t q_i[100];
	// ==============Our ECC stuff====================

	BIGNUM *alpha;
	BIGNUM *k1;
	BIGNUM *k2;
};

struct bswabe_cph_s
{
	// element_t cs; /* G_T *///M*Y^s//C1
	// element_t c;  /* G_1 *///g^s//C2
	// element_t c3; /* G_1 */
	// GArray* comps_enc; /* bswabe_enc_comp_t's */
	// bswabe_signature_t* signature;//bswabe_policy_t* p;
	mpz_t Y_m;
	mpz_t R_m;
	mpz_t C_sigma;
	// mpz_t C_m;
	mpz_t S_m;
	mpz_t e_p;

	// ============== our ECC stuff ====================
	int Policy[100]; // store policy values upto 100 values.
	EC_POINT *P_m_i[99]; // to store P_m,i where i = 1, .. n - | number of 1's in Policy[ ] |, where Policy[ ] has to have atleast 1 attribute as 1. Hence n- | Policy | is at max 99 if n = 100.
	EC_POINT *K_1m, *K_2m;  // Saving points K1m and K2m
	unsigned char *C_sigma_m, *C_m; // both will be a string of 0 and 1 of some random variable length.  
};

struct bswabe_prv_s
{

	mpz_t k1;
	mpz_t k2;
	mpz_t e_a;

	// ============= Our ECC stuff ===================
	BIGNUM *u1; 
	BIGNUM *u2; 
};

struct bswabe_sig_s
{

	element_t x;
};

struct bswabe_verification_s
{

	element_t y;
	element_t g;
	element_t g_y;
	element_t g_xy;
	char *y_s;
};

struct bswabe_signature_s
{

	element_t sigma;
};

struct bswabe_sigver_s
{

	bswabe_verification_t *a;
	bswabe_sig_t *b;
};
//proxy
struct bswabe_proxy_s
{
	element_t Cuser; /* G_1*/
	element_t Cattr;
	int on;
};
typedef struct
{
	/* these actually get serialized */
	char *attr;
	element_t d;  /* G_2 */
	element_t dp; /* G_2 */

	/* only used during dec (only by dec_merge) */
	int used;
	element_t z;  /* G_1 */
	element_t zp; /* G_1 */
} bswabe_prv_comp_t;

typedef struct
{
	int deg;
	/* coefficients from [0] x^0 to [deg] x^deg */
	element_t *coef; /* G_T (of length deg + 1) */
} bswabe_polynomial_t;

typedef struct
{
	/* serialized */
	int k;				 /* one if leaf, otherwise threshold */
	char *attr;			 /* attribute string if leaf, otherwise null */
	element_t c;		 /* G_1, only for leaves */
	element_t cp;		 /* G_1, only for leaves */
	GPtrArray *children; /* pointers to bswabe_policy_t's, len == 0 for leaves */

	/* only used during encryption */
	bswabe_polynomial_t *q;

	/* only used during decryption */
	int satisfiable;
	int min_leaves;
	int attri;
	GArray *satl;
} bswabe_policy_t;

typedef struct
{
	char *attr;
} bswabe_enc_comp_t;
