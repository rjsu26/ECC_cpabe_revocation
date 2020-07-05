#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifndef BSWABE_DEBUG
#define NDEBUG
#endif
#include <assert.h>

#include <openssl/sha.h>
#include <glib.h>
#include <pbc.h>
#include <gmp.h>

#include "bswabe.h"
#include "private.h"

#define DEBUG(arg) printf("file %s Line #%d\n", __FILE__, arg);


#define TYPE_A_PARAMS                                          \
    "type a\n"                                                 \
    "q 87807107996633125224377819847540498158068831994142082"  \
    "1102865339926647563088022295707862517942266222142315585"  \
    "8769582317459277713367317481324925129998224791\n"         \
    "h 12016012264891146079388821366740534204802954401251311"  \
    "822919615131047207289359704531102844802183906537786776\n" \
    "r 730750818665451621361119245571504901405976559617\n"     \
    "exp2 159\n"                                               \
    "exp1 107\n"                                               \
    "sign1 1\n"                                                \
    "sign0 1\n"

char last_error[256];

char *
bswabe_error()
{
    return last_error;
}

void raise_error(char *fmt, ...)
{
    va_list args;

#ifdef BSWABE_DEBUG
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    exit(1);
#else
    va_start(args, fmt);
    vsnprintf(last_error, 256, fmt, args);
    va_end(args);
#endif
}

void element_from_string(element_t h, char *s)
{
    unsigned char *r;

    r = malloc(SHA_DIGEST_LENGTH);
    SHA1((unsigned char *)s, strlen(s), r);
    element_from_hash(h, r, SHA_DIGEST_LENGTH);

    free(r);
}

int isPrime(int x)
{

    int i;
    for (i = 2; i < x; i++)
    {
        if (x % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

int powe(unsigned long int x, unsigned long int y, unsigned long int p)
{
    unsigned long int res = 1; // Initialize result

    x = x % p; // Update x if it is more than or
               // equal to p

    while (y > 0)
    {
        // If y is odd, multiply x with result
        if (y & 1)
            res = (res * x) % p;

        // y must be even now
        y = y >> 1; // y = y/2
        x = (x * x) % p;
    }
    return res;
}

void concatenate(char ans[], char *s1, char *s2, char *s3)
{
    //printf("String obtained on concatenation: %s\n", s1);
    //printf("String obtained on concatenation: %s\n", s2);
    //int size = strlen(s1) + strlen(s2) + strlen(s3) + 1;
    //char ans[size];
    //printf("%d", size);

    int i = 0;
    if (s1[0] != 'A')
    {
        while (s1[i] != '\0')
        {
            ans[i] = s1[i];
            i++;
        }
    }
    int j = i;
    i = 0;
    if (s2[0] != 'A')
    {
        while (s2[i] != '\0')
        {
            ans[j] = s2[i];
            j++;
            i++;
        }
    }

    i = 0;
    if (s3[0] != 'A')
    {
        while (s3[i] != '\0')
        {
            ans[j] = s3[i];
            j++;
            i++;
        }
    }

    // strcat(ans,s1);
    printf("String obtained on excuse me concatenation: %s\n", ans);
    // strcat(ans,s2);
    // printf("String obtained on concatenation: %s\n", ans);
    // strcat(ans, s3);
    // printf("String obtained on concatenation: %s\n", ans);
    return;
}

void take_Concatenate(char sol[], mpz_t m1, mpz_t m2, mpz_t m3)
{

    char s1[100];
    char s2[100];
    char s3[100];

    int flag = 0;
    int flag2 = 0;
    int flag3 = 0;
    if (mpz_cmp_ui(m1, 0) == 0)
    {
        s1[0] = "A";
    }
    else
    {
        mpz_get_str(s1, 10, m1);
        flag = strlen(s1);
    }

    if (mpz_cmp_ui(m2, 0) == 0)
    {
        s2[0] = "A";
    }
    else
    {
        mpz_get_str(s2, 10, m2);
        flag2 = strlen(s2);
    }

    if (mpz_cmp_ui(m3, 0) == 0)
    {
        s3[0] = "A";
    }
    else
    {
        mpz_get_str(s3, 10, m3);
        flag3 = strlen(s3);
    }

    int size = flag + flag2 + flag3;
    char ans[size];
    concatenate(ans, s1, s2, s3);
    int i;
    for (i = 0; i < size; i++)
    {
        sol[i] = ans[i];
    }
}

unsigned long int generate_random(mpz_t rand_Num, int limit)
{

    unsigned long int seed = rand() % limit;
    mpz_init_set_ui(rand_Num, seed);

    return seed;
}

void compute_hash(mpz_t hash, char str[])
{

    mpz_init(hash);
    mpz_t p;
    mpz_init_set_ui(p, 2);
    mpz_t mod;
    mpz_init_set_ui(mod, 100);
    //printf("working");
    char c;

    for (c = 0; c < strlen(str); c++)
    {
        mpz_t m;
        mpz_init(m);
        mpz_t ans;
        mpz_init(ans);

        mpz_pow_ui(m, p, c);
        mpz_mod(m, m, mod);
        mpz_t vall;
        int x = str[c] - '0';

        mpz_init_set_ui(vall, x);

        mpz_mul(ans, vall, m);
        mpz_mod(ans, ans, mod);
        mpz_add(hash, hash, ans);
        mpz_mod(hash, hash, mod);
    }

    gmp_printf("//////////////////%Zd//////////////////", hash);
    return;
}

void compute_hash2(mpz_t hash, unsigned long int x)
{

    unsigned long int num = x;
    mpz_init(hash);
    mpz_t p;
    mpz_init_set_ui(p, 2);
    mpz_t mod;
    mpz_init_set_ui(mod, 100);
    //printf("working");
    int c = 0;

    while (num != 0)
    {
        mpz_t m;
        mpz_init(m);
        mpz_t ans;
        mpz_init(ans);

        mpz_pow_ui(m, p, c);
        mpz_mod(m, m, mod);
        mpz_t vall;
        unsigned long int x_ = num % 10;

        mpz_init_set_ui(vall, x_);

        mpz_mul(ans, vall, m);
        mpz_mod(ans, ans, mod);
        mpz_add(hash, hash, ans);
        mpz_mod(hash, hash, mod);
        c++;
        num = num / 10;
    }

    gmp_printf("//////////////////%Zd//////////////////", hash);
    return;
}

int *delete_subarr(int arr_primes[], int arr_remove[], int n, int m)
{
    // int arr_remove[m]; // size = m
    int *ans_array;
    ans_array = malloc(sizeof(int) * (n - m));
    int i, j = 0;
    int k = 0;
    for (i = 0; i < n; i++)
    {
        int flag = 0;
        for (j = 0; j < m; j++)
        {
            if (arr_remove[j] == arr_primes[i])
            {
                flag = 1;
            }
        }

        if (flag == 0)
        {
            ans_array[k] = arr_primes[i];
            k++;
        }
    }
    return ans_array;
}

// NEW
// Take all parameters of ECC and return an EC_GROUP
EC_GROUP *create_curve(BIGNUM* a,BIGNUM* b,BIGNUM* p,BIGNUM* order,BIGNUM* x,BIGNUM* y){

    BN_CTX *ctx;
    ctx = BN_CTX_new();
    EC_GROUP *curve;
    curve = EC_GROUP_new_curve_GFp(p, a, b, ctx);
    EC_POINT *generator;
    generator = EC_POINT_new(curve);
    EC_POINT_set_affine_coordinates_GFp(curve, generator, x, y, ctx);
    EC_GROUP_set_generator(curve, generator, order, NULL);
    EC_POINT_free(generator);
    BN_CTX_free(ctx); 

    return curve;
}

void bswabe_setup(bswabe_pub_t **mpk, bswabe_msk_t **msk, int n)
{
    *mpk = malloc(sizeof(bswabe_pub_t));
    *msk = malloc(sizeof(bswabe_msk_t));
    (*mpk)->n =  n;
    BN_CTX *ctx;
    if (NULL == (ctx = BN_CTX_new()))
        printf("error\n");

   unsigned char a_bin[32] = {
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
    };
     unsigned char b_bin[32] = {
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x07, 
    };
    unsigned char p_bin[32] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFC, 0x2F};
    unsigned char order_bin[32] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xBA, 0xAE, 0xDC, 0xE6, 0xAF, 0x48, 0xA0, 0x3B, 0xBF, 0xD2, 0x5E, 0x8C, 0xD0, 0x36, 0x41, 0x41};
    unsigned char x_bin[32] = {0x79, 0xbe, 0x66, 0x7e, 0xf9, 0xdc, 0xbb, 0xac, 0x55, 0xa0, 0x62, 0x95, 0xce, 0x87, 0x0b, 0x07, 0x02, 0x9b, 0xfc, 0xdb, 0x2d, 0xce, 0x28, 0xd9, 0x59, 0xf2, 0x81, 0x5b, 0x16, 0xf8, 0x17, 0x98};
    unsigned char y_bin[32] = {0x48, 0x3a, 0xda, 0x77, 0x26, 0xa3, 0xc4, 0x65, 0x5d, 0xa4, 0xfb, 0xfc, 0x0e, 0x11, 0x08, 0xa8, 0xfd, 0x17, 0xb4, 0x48, 0xa6, 0x85, 0x54, 0x19, 0x9c, 0x47, 0xd0, 0x8f, 0xfb, 0x10, 0xd4, 0xb8};
    
    if(NULL == ((*mpk)->a = BN_bin2bn(a_bin, 32, NULL)))
        printf("ERROR a_bin\n");
    if(NULL == ((*mpk)->b = BN_bin2bn(b_bin, 32, NULL)))
        printf("ERROR b_bin\n");    
    if(NULL == ((*mpk)->p = BN_bin2bn(p_bin, 32, NULL)))
        printf("ERROR p_bin\n");  
    if(NULL == ((*mpk)->order = BN_bin2bn(order_bin, 32, NULL)))
        printf("ERROR order\n");
    if(NULL == ((*mpk)->G_x = BN_bin2bn(x_bin, 32, NULL)))
        printf("ERROR G_x\n");   
    if(NULL == ((*mpk)->G_y = BN_bin2bn(y_bin, 32, NULL)))
        printf("ERROR G_y\n");


    BIGNUM *p_3;
    p_3 = BN_new(); // p_3 : p
    if(NULL == BN_copy(p_3, (*mpk)->p))
        printf("ERROR p_3\n");

    BN_sub_word(p_3,1);
    BN_sub_word(p_3,1);
    BN_sub_word(p_3,1); // p_3 : p - 3


// Generate random number between [0,p-4]
    BIGNUM *temp;
    temp = BN_new();
    BN_rand_range(temp,p_3); 
    (*msk)->alpha = BN_dup(temp);
    BN_rand_range(temp,p_3); 
    (*msk)->k1 = BN_dup(temp);
    BN_rand_range(temp,p_3);
    (*msk)->k2 = BN_dup(temp);
    BN_free(p_3);
    // add 1 to each for two times to make the range : [2, p-2]. Avoiding p-1 because (p-1)G gives infinity point.
    for(int i=0;i<2;i++){
        BN_add_word((*msk)->alpha, 1);
        BN_add_word((*msk)->k1, 1);
        BN_add_word((*msk)->k2, 1);
    } 

    // Create curve for calculation of P_i[], U_i[], V_i[]
    EC_GROUP *curve= create_curve((*mpk)->a,(*mpk)->b,(*mpk)->p,(*mpk)->order,(*mpk)->G_x,(*mpk)->G_y);
    EC_POINT* temp_point;
    temp_point = EC_POINT_new(curve);

    // Initializing P_i[0] = G and U_i[0] = k1* G and V_i[0] = k2*G
    (*mpk)->P_i[0]= EC_POINT_dup(EC_GROUP_get0_generator(curve), curve);
    // EC_POINT_copy((*mpk)->P_i[0],EC_GROUP_get0_generator(curve));
    EC_POINT_mul(curve,temp_point ,(*msk)->k1,NULL, NULL, ctx);
    (*mpk)->U_i[0] = EC_POINT_dup(temp_point, curve);
    EC_POINT_mul(curve, temp_point,(*msk)->k2,NULL, NULL, ctx);
    (*mpk)->V_i[0] = EC_POINT_dup(temp_point, curve);
    // P_i[k] = alpha * P_i[k-1]
    // U_i[k] =  k1* P_i[k] 
    // V_i[k] =  k2* P_i[k]

    for(int j=1;j<n;j++){
        EC_POINT_mul(curve, temp_point,NULL, (*mpk)->P_i[j-1],(*msk)->alpha, ctx);
        (*mpk)->P_i[j] = EC_POINT_dup(temp_point, curve);
        EC_POINT_mul(curve, temp_point,NULL, (*mpk)->P_i[j],(*msk)->k1, ctx);
        (*mpk)->U_i[j] = EC_POINT_dup(temp_point, curve);
        EC_POINT_mul(curve, temp_point,NULL, (*mpk)->P_i[j],(*msk)->k2, ctx);
        (*mpk)->V_i[j] = EC_POINT_dup(temp_point, curve);
    }
        
    return;
}

bswabe_cph_t *
bswabe_enc(bswabe_pub_t *pub, bswabe_msk_t *msk, element_t m, int attrib[])
{

    //#######################################ODELU####################################################
    pairing_t px;
    char *pairing_desc;
    pairing_desc = strdup(TYPE_A_PARAMS);
    pairing_init_set_buf(px, pairing_desc, strlen(pairing_desc));
    element_init_GT(m, px);
    element_random(m);
    element_printf("%B", m);

    mpz_t x;
    mpz_init_set_ui(x, 10);
    mpz_t HYPER_MOD;
    mpz_init(HYPER_MOD);
    mpz_pow_ui(HYPER_MOD, x, 100000);
    mpz_t p_1;
    mpz_t q_1;
    mpz_init(p_1);
    mpz_init(q_1);
    mpz_sub_ui(p_1, msk->p, 1);
    mpz_sub_ui(q_1, msk->q, 1);
    printf("working");
    mpz_t totient;
    mpz_init(totient);
    mpz_mul(totient, p_1, q_1);

    bswabe_cph_t *cph;

    cph = malloc(sizeof(bswabe_cph_t));

    mpz_t mod;
    mpz_init_set_ui(mod, 1000000000);

    mpz_t blank1;
    mpz_init(blank1);

    mpz_t blank2;
    mpz_init(blank2);

    mpz_t r_m;
    generate_random(r_m, 30);
    //mpz_init_set_ui(r_m,1);
    gmp_printf("\nvalue of pub->Y is %Zd and r_m in encrypt is = %Zd\n", pub->Y, r_m);
    // fully correct ab misc.c ki ek copy bana
    //mpz_init(r_m);
    //char ans[100];
    //printf("FIRST HASHING");
    //take_Concatenate(ans, M, sigma,blank1);
    // working on attrib and string concatenation

    //printf("%s\n", ans);
    //	int size = pub -> n;
    //	int j = strlen(ans);
    //	int final_length = j + size;
    //	char final_ans[final_length];
    /* fix it later
	for(int i = 0;i < final_length;i++){

	if(i < j){
		final_ans[i] = ans[i]; 

	}else{

		if(attrib[i] == 1){
			final_ans[i] = '1';
		}
		else{
			final_ans[i] = '0'; 
		} 
	}
	}*/

    //printf("######## %s\n",ans);

    //compute_hash(r_m, ans);

    //gmp_printf("%Zd",r_m);

    mpz_t e_u;
    mpz_init_set_ui(e_u, 1);

    mpz_t e_p;
    mpz_init_set_ui(e_p, 1);
    //int n = 4;
    int n = mpz_get_ui(pub->n);
    //  printf("\n\\\\%d\\\\n",n);
    int i;
    for (i = 0; i < n; i++)
    {
        if (attrib[i] == 1)
        {
            mpz_mul(e_p, e_p, pub->p_i[i]);
        }
        mpz_mul(e_u, e_u, pub->p_i[i]);
    }
    mpz_init_set(cph->e_p, e_p);

    // working on C_sigma and C_m and S_m
    mpz_t inter;
    mpz_init_set(inter, pub->D_u);

    mpz_cdiv_q(e_u, e_u, e_p);
    //mpz_mod(e_u,e_u,totient);
    //mpz_mod(r_m,r_m,totient);
    //mpz_t x_;
    //mpz_t y;
    //mpz_init(x_);
    //mpz_init(y);
    //mpz_mul(x_, e_u, r_m);//eu*ru
    //mpz_mod(x_,x_,totient);
    //mpz_mul(y,x_,pub -> d_u);
    //mpz_mod(y, y, totient);
    mpz_powm(inter, inter, r_m, pub->N);
    mpz_powm(inter, inter, e_u, pub->N);

    /*mpz_t modx;
	mpz_init(modx);
	mpz_mod(modx, e_u, totient);*/
    gmp_printf("!!!!!!!!%Zd$$$$$$$$$$$$$$$%Zd!!!!!!!!!!!!!!!\n", e_u, inter);

    //mpz_powm(inter, inter, r_m, mod);
    //mpz_powm(inter, inter, e_u, mod);

    mpz_t H2_K_m;
    mpz_init(H2_K_m);

    mpz_t sigma;
    generate_random(sigma, 30);
    //gmp_printf("^^^^^^ %Zd" , sigma);

    // -----------

    printf("\nSECOND HASH");
    //  gmp_printf("%Zd ******************* %Zd", blank1,blank2);
    char ans2[100];

    mpz_t blank3;
    mpz_t blank4;
    mpz_init(blank3);
    mpz_init(blank4);
    //take_Concatenate(ans2, inter,blank3,blank4);
    mpz_get_str(ans2, 10, inter); // chala
    //gmp_printf("%Zd ******************* %Zd", blank1,blank2);

    printf("%s---------------", ans2);
    compute_hash(H2_K_m, ans2);
    gmp_printf("k_m in encrypt is %Zd", H2_K_m);

    mpz_init(cph->C_sigma);
    mpz_xor(cph->C_sigma, H2_K_m, sigma);

    gmp_printf("\n\nvalue of cph -> C_sigma = %Zd\n\n", cph->C_sigma);
    mpz_t H3_sigma;
    mpz_init(H3_sigma); //

    //char ans3[1000];
    //take_Concatenate(ans3, sigma, blank1 , blank2);
    //mpz_get_str(ans3, 10, sigma);
    unsigned long int ans3 = mpz_get_ui(sigma);
    //gmp_printf("\n\nvalue of sigma is %Zd and blank1 is %Zd\n\n",sigm);

    printf("((((((((((((((((%d))))))))))))))", ans3);
    compute_hash2(H3_sigma, ans3);

    mpz_t M;
    generate_random(M, 30);
    gmp_printf("\n\nvalue of M is %Zd and value of H3_sigma is %Zd\n\n", M, H3_sigma);
    mpz_init(cph->C_m);
    //unsigned long int value_M = mpz_get_ui(M);
    //unsigned long int value_H3 = mpz_get_ui(H3_sigma);
    //unsigned long int value_ans = value_M ^ value_H3;
    //printf("M = %d and H3 = %d and ans = %d", value_M, value_H3, value_ans);
    //mpz_init_set_ui(cph -> C_m, value_ans);

    mpz_xor(cph->C_m, M, H3_sigma);

    gmp_printf("\n\n3rd hash completed and value of cph -> C_m is %Zd\n\n", cph->C_m);
    char ans4[4];
    unsigned long int M_v = mpz_get_ui(M);
    unsigned long int sigma_v = mpz_get_ui(sigma);

    unsigned long int final_ans = sigma_v * 100 + M_v;
    //take_Concatenate(ans4, sigma, M, blank1); sigma = 15 && M = 23 sigma * 100 + 23 + 523

    // 1627 + 1 + 12 + 16 + 56
    printf("\n\n value of ans4 is %s\n\n", ans4);

    compute_hash2(cph->S_m, final_ans);
    printf("4th Hash Completed");
    // working on Y_m and R_m

    //	mpz_t x_mod;
    //	mpz_init(x_mod);
    //	mpz_mod(x_mod, msk -> x, totient);

    //	mpz_t k_mod;
    //	mpz_init(k_mod);
    //	mpz_mod(k_mod, msk -> k, totient);

    //	mpz_t z;
    //	mpz_init(z);
    //	mpz_mul(z,x_mod, r_m);
    //	mpz_mod(z,z,totient);
    //      gmp_printf("z is = %Zd\n",z);

    //	mpz_t z1;
    //	mpz_init(z1);
    //	mpz_mul(z1,k_mod, r_m);
    //	mpz_mod(z1,z1,totient);

    mpz_init(cph->Y_m);
    mpz_init(cph->R_m);
    mpz_powm(cph->Y_m, pub->Y, r_m, pub->N);
    gmp_printf("Y_m in encrypt is = %Zd\n", cph->Y_m);
    unsigned long int pub_R = mpz_get_ui(pub->R);
    printf("pub_R in encrypt is = %lu\n", pub_R);
    unsigned long int ir_m = mpz_get_ui(r_m);
    printf("ir_m in encrypt is = %lu\n", ir_m);
    unsigned long int pub_N = mpz_get_ui(pub->N);
    printf("pub_N in encrypt is = %lu\n", pub_N); // specifier
    unsigned long int cph_Rm = powe(pub_R, ir_m, pub_N);
    mpz_init_set_ui(cph->R_m, cph_Rm);
    //mpz_powm(cph -> R_m, pub -> R, r_m, pub -> N);
    gmp_printf("R_m in encrypt is = %Zd\n", cph->R_m);
    return cph;
}

// f(alpha, attrubutes) = PI((alpha + hash4(i))^(1-a_i)) , where i = 1 to n
BIGNUM *f(BIGNUM *x,int *attributes,int n) {
  BN_CTX *ctx = BN_CTX_new();
  BIGNUM *res = BN_new();
  BN_one(res);
  BIGNUM *tmp = BN_new();
  unsigned long int temp;

  for(int i=0;i<n;i++) 
    if(attributes[i]==0) {
      temp = hash4(i+1); // TODO
      // BN_zero(tmp);
      BN_copy(tmp,x);
      BN_add_word(tmp,temp);
      BN_mul(res,res,tmp,ctx);
    }
  
  BN_CTX_free(ctx);
  return res;
}


bswabe_prv_t *bswabe_keygen(bswabe_pub_t *pub, bswabe_msk_t *msk, int user_attr_set[])
{
    bswabe_prv_t *prv;
    prv = malloc(sizeof(bswabe_prv_t));
    BN_CTX *ctx = BN_CTX_new();

    BIGNUM *f_alpha = f(msk->alpha, user_attr_set,pub->n); // findinf f(alpha, attributes)
    BIGNUM *s_u = BN_new();
    BIGNUM *t_u = BN_new();
    BIGNUM *r_u = BN_new();

    // Initialise r_u and t_u with random values 
    BN_rand_range(r_u, pub->p); 
    BN_rand_range(t_u, pub->p);

    BIGNUM *k1_inv = BN_new();
    BIGNUM *f_inv = BN_new();

    // Evaluating s_u
    BN_mod_inverse(k1_inv, msk->k1, pub->p, ctx); // k1_inv = (1/k1) mod p
    BN_mod_inverse(f_inv, f_alpha, pub->p, ctx); // f_inv = (1/f_alpha) mod p
    BN_mod_mul(s_u, msk->k2, r_u, pub->p, ctx); // s_u = k2 * r_u
    BN_mod_sub(s_u,f_inv,s_u,pub->p,ctx); // s_u = (1/f_alpha) - k2* r_u
    BN_mod_mul(s_u, s_u, k1_inv, pub->p, ctx); // s_u = (1/k1) * s_u

    // Finding u1
    BN_mod_mul(prv->u1,msk->k1,t_u,pub->p,ctx); // u1 = k1 * t_u
    BN_mod_add(prv->u1, r_u, prv->u1, pub->p, ctx); // u1 = r_u + k1*t_u (mod p)

    // Finding u2
    BN_mod_mul(prv->u2, msk->k2, t_u, pub->p, ctx); // u2 = k2* t_u
    BN_mod_sub(prv->u2, s_u, prv->u2, pub->p, ctx); // u2 = s_u - k2*t_u (mod p)

    BN_free(f_alpha);
    BN_free(s_u);
    BN_free(r_u);
    BN_free(t_u);
    BN_free(k1_inv);
    BN_free(f_inv);
    BN_CTX_free(ctx);
    return prv;
}

void bswabe_proxy(mpz_t k1, mpz_t C_attr, mpz_t C_user)
{

    mpz_t random;

    generate_random(random, 30);

    mpz_t random2;
    generate_random(random2, 30);
    mpz_add_ui(random2, random, 61);
    mpz_init(C_attr);
    int option;
    printf("PRESS 0 FOR NO REVOCATION AND 1 FOR USER REVOCATTION ");
    scanf("%d", &option);

    mpz_init_set(C_user, random);

    if (option == 0)
    {
        mpz_mul(C_attr, k1, random); //NR
    }
    else
    {

        mpz_mul(C_attr, k1, random2);
    }
}

int bswabe_dec(bswabe_pub_t *pub, bswabe_prv_t *prv, bswabe_cph_t *cph, element_t m) // attrib is P
{
    pairing_t px;
    char *pairing_desc;
    pairing_desc = strdup(TYPE_A_PARAMS);
    pairing_init_set_buf(px, pairing_desc, strlen(pairing_desc));
    element_init_GT(m, px);
    element_random(m);
    mpz_t x;
    mpz_init_set_ui(x, 10);
    mpz_t HYPER_MOD;
    mpz_init(HYPER_MOD);
    mpz_pow_ui(HYPER_MOD, x, 100000); // 10^1000

    mpz_t mod;
    mpz_init_set_ui(mod, 100000000);
    gmp_printf("value of k1 in decrypt is %Zd \n", prv->k1);
    // WORKING ON D1
    mpz_t product1;
    mpz_init(product1);
    mpz_t product2;
    mpz_init(product2);
    mpz_t product3;
    mpz_init(product3);
    mpz_t a;
    mpz_init(a);
    mpz_t expo;
    mpz_init(expo);
    mpz_t K_m;
    mpz_init(K_m);
    mpz_t C_attr;
    mpz_t C_user; // init
    mpz_init(C_attr);
    mpz_init(C_user);
    bswabe_proxy(prv->k2, C_attr, C_user);
    gmp_printf("@value of c_attr  is %Zd , %Zd \n", C_attr, C_user);
    mpz_t expo2;
    mpz_init(expo2);
    mpz_cdiv_q(expo2, C_attr, C_user);
    gmp_printf("\nCATTR, CUSER, Exponent 2 : %Zd %Zd %Zd\n", C_attr, C_user, expo2);

    unsigned long int K_mi;
    unsigned long int final_Km = 0;

    if (mpz_divisible_p(prv->e_a, cph->e_p) != 0)
    {
        mpz_divexact(expo, prv->e_a, cph->e_p);
        unsigned long int a_p = mpz_get_ui(expo);
        unsigned long int pub_N = mpz_get_ui(pub->N);
        gmp_printf("value of pubn before in decrypt == %Zd\n", pub->n);
        unsigned long int cph_Rm = mpz_get_ui(cph->R_m);
        printf("value of cph_rm in decrypt from integer approach is %lu\n", cph_Rm);
        unsigned long int cph_Ym = mpz_get_ui(cph->Y_m);
        printf("value of cph_ym in decrypt from integer approach is %lu\n", cph_Ym);
        unsigned long int prv_k1 = mpz_get_ui(prv->k1);
        printf("value of prv_k1 in decrypt from integer approach is %lu\n", prv_k1);

        unsigned long int prv_k2 = mpz_get_ui(expo2);
        printf("value of prv_k2 in decrypt from integer approach is %lu\n", prv_k2);
        unsigned long int Rm_k1 = powe(cph_Rm, prv_k1, pub_N);
        printf("value of Rm_k1 in decrypt from integer approach is %lu\n", Rm_k1);
        unsigned long int Ym_k2 = powe(cph_Ym, prv_k2, pub_N);
        printf("value of Ym_k2 in decrypt from integer approach is %lu\n", Ym_k2);
        K_mi = (Rm_k1 * Ym_k2) % pub_N;
        final_Km = powe(K_mi, a_p, pub_N);

        printf("value of Rm_k1 in decrypt from integer approach is %lu\n", Rm_k1);
        printf("value of Ym_k2 in decrypt from integer approach is %lu\n", Ym_k2);
        printf("value of K_mi in decrypt from integer approach is %lu\n", K_mi);

        gmp_printf("value of Y_m in decrypt == %Zd\n", cph->Y_m);
        gmp_printf("value of R_m in decrypt == %Zd\n", cph->R_m);

        gmp_printf("value of c_attr 2 is %Zd \n", C_attr);
        mpz_powm(product1, cph->Y_m, expo2, pub->N);
        gmp_printf("value of product1 in decrypt == %Zd\n", product1);
        gmp_printf("value of K1 in decrypt == %Zd\n", prv->k1);
        mpz_powm(product2, cph->R_m, prv->k1, pub->N);
        gmp_printf("value of product2 in decrypt == %Zd\n", product2);
        mpz_mul(product3, product1, product2);
        gmp_printf("value of product3 in decrypt == %Zd\n", product3);
        mpz_mod(a, product3, pub->N);

        gmp_printf("value of expo in decrypt == %Zd\n", expo);
        mpz_powm(K_m, a, expo, pub->N);
        gmp_printf("value of K_m in decrypt == %Zd\n", K_m);
    }

    else
    {
        printf("computation of K_m is computationally infeasible");
        return 0;
    }

    gmp_printf(" e_a is = %Zd", prv->e_a);
    gmp_printf(" e_p is = %Zd", cph->e_p);
    // WORKING ON D2

    // Compute σ'm= H2(Km) ⊕ C_σm and M'= Cm ⊕ H3 (σ'm )
    mpz_t sigma_d_m;
    mpz_init(sigma_d_m);
    mpz_t M_d;
    mpz_init(M_d);
    mpz_t blank1;
    mpz_init(blank1);
    mpz_t blank2;
    mpz_init(blank2);

    mpz_t H2_K_m;
    mpz_init(H2_K_m);
    mpz_init_set_ui(K_m, final_Km);
    char str1[10000];
    //take_Concatenate(str1,K_m,blank1,blank2);
    mpz_get_str(str1, 10, K_m);
    //printf("hello i am  == %s", str1);
    compute_hash(H2_K_m, str1);

    mpz_xor(sigma_d_m, H2_K_m, cph->C_sigma);
    gmp_printf("value of sigma_d_m = %Zd", sigma_d_m);
    mpz_t H3_sigma_m;
    mpz_init(H3_sigma_m);

    //take_Concatenate(str2,sigma_d_m,blank1,blank2);
    int str2 = mpz_get_ui(sigma_d_m);
    compute_hash2(H3_sigma_m, str2);

    mpz_xor(M_d, cph->C_m, H3_sigma_m);

    gmp_printf("value of M in decrypt is %Zd", M_d);

    mpz_t S_m;
    mpz_init(S_m);

    mpz_t H1;
    mpz_init(H1);

    char str3[10000];
    take_Concatenate(str3, sigma_d_m, M_d, blank1);
    unsigned long int M_v = mpz_get_ui(M_d);
    unsigned long int sigma_v = mpz_get_ui(sigma_d_m);

    unsigned long int final_ans = sigma_v * 100 + M_v;
    printf("\n\n str3 is %s\n\n", str3);
    compute_hash2(H1, final_ans);

    if (mpz_cmp(cph->S_m, H1) == 0)
    {
        return 1;
    }

    return 0;
}
