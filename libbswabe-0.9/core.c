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
    for (i = 2; i*i <= x; i++)
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
    //printf("String obtained on excuse me concatenation: %s\n", ans);
    // strcat(ans,s2);
    // printf("String obtained on concatenation: %s\n", ans);
    // strcat(ans, s3);
    // printf("String obtained on concatenation: %s\n", ans);
    return;
}

// old : to be deleted later
void take_Concatenate(char sol[], mpz_t m1, mpz_t m2, mpz_t m3){
    return;
}

char* take_Concatenate_new( BIGNUM *m1, BIGNUM *m2, BIGNUM *m3)
{

    char *s1;
    char *s2;
    char *s3;
    // DEBUG(__LINE__);
    s1 = BN_bn2dec(m1);
    s2 = BN_bn2dec(m2);
    s3 = BN_bn2dec(m3);
    // BN_print_fp(stdout, m1); printf("\n");
    // printf("%s \n", s1);
    // BN_print_fp(stdout, m2); printf("\n");
    // printf("%s \n", s2);
    // BN_print_fp(stdout, m3); printf("\n");
    // printf("%s \n", s3);

    char *ans = (char*)calloc(500,sizeof(char)) ;
    strcpy(ans,s1);
    strcat(ans,s2);
    strcat(ans,s3);
    return ans;
    // int flag = 0;
    // int flag2 = 0;
    // int flag3 = 0;
    
    // strcpy(s1,m1);

    // if (BN_is_zero(m1) == 1)
    // {
    //     s1 = "A";
    // }
    // else
    // {
    //     s1 = BN_bn2dec(m1);
    //     //printf(" s2  ->  %s",s2);
    //     flag1 = strlen(s1);
    // }

    // if (BN_is_zero(m2) == 1)
    // {
    //     s2 = "A";
    // }
    // else
    // {
    //     s2 = BN_bn2dec(m2);
    //     //printf(" s2  ->  %s",s2);
    //     flag2 = strlen(s2);
    // }

    // if (BN_is_zero(m3) == 1)
    // {
    //     s3 = "A";
    // }
    // else
    // {
    //     s3 = BN_bn2dec(m3);
    //    // printf(" s3  ->  %s",s3);
    //     flag3 = strlen(s3);
    // }

    // int size = flag + flag2 + flag3;
    // char ans[size];
    // concatenate(ans, s1, s2, s3);   // concatenate 
    // int i;
    // for (i = 0; i < size; i++)
    // {
    //     sol[i] = ans[i];
    // }
}

// old function re-wrote to remove compilation error
void compute_hash(mpz_t hash, char str[]){
    return;
}

void compute_hash_new(BIGNUM *hash, char *str)
{
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *p = BN_new();
    BN_set_word(p, 2);
    BIGNUM *mod = BN_new();
    BN_set_word(mod, 100);
    //printf("working");
    unsigned int c;
    BIGNUM *temp = BN_new();
    for (c = 0; c < strlen(str); c++)
    {
        BIGNUM *m = BN_new();
        //mpz_init(m);
        BIGNUM *ans = BN_new();
        //mpz_init(ans);
        BN_set_word(temp,c);
        BN_exp(m, p, temp,ctx);
        BN_mod(m, m, mod,ctx);
        BIGNUM *vall = BN_new();
        BIGNUM *x = BN_new();
        BN_set_word(x,str[c]-'0');

        BN_copy(vall, x);

        BN_mul(ans, vall, m,ctx);
        BN_mod(ans, ans, mod,ctx);
        BN_add(hash, hash, ans);
        BN_mod(hash, hash, mod,ctx);
    }

    //gmp_printf("//////////////////%Zd//////////////////", hash);
    BN_CTX_free(ctx);
    return;
}

// old function re-wrote to remove compilation error
unsigned long int generate_random(mpz_t rand_Num,int limit){
    unsigned long int x=0;
    return x;
}

// NEW
void compute_hash2_new(BIGNUM *hash, BIGNUM *x)
{
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *num = BN_new();
    BN_copy(num,x);
    BIGNUM *p = BN_new();
    BN_set_word(p,2);
    //mpz_init_set_ui(p, 2);
    BIGNUM *mod = BN_new();
    BN_set_word(mod,100);
    //printf("working");
    BIGNUM *c = BN_new();
    BN_set_word(c,0);
    
    while (BN_is_zero(num)==0)
    {
        BIGNUM *m = BN_new();
        BIGNUM *ans = BN_new();

        BN_exp(m, p, c,ctx);
        BN_mod(m, m, mod,ctx);
        BIGNUM *vall = BN_new();
        BIGNUM *ten = BN_new();
        BN_set_word(ten,10);
        BIGNUM *x_ = BN_new();
        BN_mod(x_,num,ten,ctx);
        BN_copy(vall,x);
        BN_mul(ans, vall, m,ctx);
        BN_mod(ans, ans, mod,ctx);
        BN_add(hash, hash, ans);
        BN_mod(hash, hash, mod,ctx);
        BN_add_word(c,1);
        BN_div_word(num,10);
    }
}

unsigned long int hash_4(unsigned long int i) {
  return i;
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
BIGNUM *f(BIGNUM *x,int *attributes,int n) {
  DEBUG(__LINE__);
  BN_CTX *ctx = BN_CTX_new();
  BIGNUM *res = BN_new();
  BN_one(res);
  //BIGNUM *sum = BN_new();
  BIGNUM *tmp = BN_new();
  unsigned long int temp;
  for(int i=0;i<n;i++) {
    if(attributes[i]==0) {
      temp = hash_4(i+1);
      BN_zero(tmp);
      BN_copy(tmp,x);
      BN_add_word(tmp,temp);
      BN_mul(res,res,tmp,ctx);
    }
  DEBUG(__LINE__);
  }
  BN_CTX_free(ctx);
  return res;
}


//
void print_bignum(BIGNUM * num) {
    BN_print_fp(stdout, num);
    fprintf(stdout, "\n");
}

char *binOfbn(BIGNUM *b) {
    unsigned char *binary = (unsigned char*) OPENSSL_malloc(BN_num_bytes(b)*sizeof(unsigned char));
  int len = BN_bn2bin(b, binary);
  char *to = (char*) OPENSSL_malloc((len*8+2)*sizeof(char));
  int offset = 0;
  if (BN_is_negative(b)) {
    to[0] = '-';
    offset--;
  }
  unsigned char x = binary[0];
  while (!(x & 128) && x) {
    x = x << 1;
    offset++;
  }
  for (int i = 0; i < len; i++) {
    unsigned char bits = binary[i];

    int j=7;
    while(bits) {
      if (bits & 1) {
        to[8*i+j-offset] = '1';
      } else {
        to[8*i+j-offset] = '0';
      }
      bits = bits >> 1;
      j--;
    }
    if (i > 0) {
      while (j >= 0) {
        to[8*i+j-offset] = '0';
        j--;
      }
    }
  }
  to[8*len-offset] = '\0';
  OPENSSL_free(binary);
  return to;
}

////    ECC CURVE
void print_point(EC_GROUP *curve, EC_POINT *point, BN_CTX *ctx) {

    BIGNUM *x = BN_new();
    BIGNUM *y = BN_new();

    if (EC_POINT_get_affine_coordinates_GFp(curve, point, x, y, ctx)) {
        fprintf(stdout, "     x = 0x");
        BN_print_fp(stdout, x);
        fprintf(stdout, "\n     y = 0x");
        BN_print_fp(stdout, y);
        fprintf(stdout, "\n\n");
    } else {
        printf("Error in printing (x,y)\n");
    }
}


void handleErrors(int lineNumber) {
    printf("Error at LINE=%d\n", lineNumber);
}

// NEW
// Take all parameters of ECC and return an EC_GROUP
EC_GROUP *create_curve(BIGNUM* a,BIGNUM* b,BIGNUM* p,BIGNUM* order,BIGNUM* x,BIGNUM* y){

    BN_CTX *ctx;
    ctx = BN_CTX_new();
    EC_GROUP *curve;
    // DEBUG(__LINE__);
    curve = EC_GROUP_new_curve_GFp(p, a, b, ctx);
    EC_POINT *generator;
    //DEBUG(__LINE__);
    generator = EC_POINT_new(curve);
    EC_POINT_set_affine_coordinates_GFp(curve, generator, x, y, ctx);
    //DEBUG(__LINE__);
    EC_GROUP_set_generator(curve, generator, order, NULL);
    EC_POINT_free(generator);
    //DEBUG(__LINE__);
    BN_CTX_free(ctx); 

    return curve;
}
void multi(unsigned long long int *arr,int n,int a) {
  //mul with x
  unsigned long long int *temp = (unsigned long long int*)calloc(n+1,sizeof(unsigned long long int));
  for(int i=n;i>=0;i--){
    temp[i] = arr[i]*a;
  }
  for(int i=n-1;i>=0;i--) {
    arr[i+1] = arr[i];
  }
  arr[0] = 0;
  for(int i=0;i<=n;i++) {
    arr[i] = arr[i] + temp[i];
  }
  free(temp);
}
unsigned long long int coefficients(int pol[],int n) {
  unsigned long long int *poly = (unsigned long long int*)calloc(n+1,sizeof(unsigned long long int));
  for(int i=0;i<=n;i++) {
    poly[i] = 0;
  }
  poly[0]=1;
  for(int i=0;i<n;i++) {
    if(pol[i]==0){
      multi(poly,n,hash_4(i+1));
    }
  }
  return poly;
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

// old: to be deleted
bswabe_cph_t* bswabe_enc( bswabe_pub_t* pub, bswabe_msk_t* msk, char* m, int attrib[]){
    bswabe_cph_t * cph  = (bswabe_cph_t*)malloc(sizeof(bswabe_cph_t));
    return cph;
}


// NEW
bswabe_cph_t *
bswabe_enc_new(bswabe_pub_t *pub, bswabe_msk_t *msk, BIGNUM* M, int attributes[])
{


    BN_CTX *ctx = BN_CTX_new();
    // making policy ready for concatenation

    //creating curve
    EC_GROUP *curve = create_curve(pub->a,pub->b,pub->p,pub->order,pub->G_x,pub->G_y);

    BIGNUM *pol = BN_new();
    BN_zero(pol);
    // BN_print_fp(stdout, pol);printf("\n");
    for(int i=0;i<pub->n;i++) {
        BN_mul_word(pol,10);
        BN_add_word(pol,attributes[i]);
    }

    // DEBUG(__LINE__);
    // BN_print_fp(stdout, pol);printf("\n");
    // message M
    // BIGNUM *M = BN_new();
    // BN_dec2bn(&M,m);
    // for(int i=0;i<32;i++){
    //     printf("%d ", m[i] );
    // }
    // printf("\n");

    // BIGNUM *M= BN_bin2bn(m, sizeof(m), NULL);
    // BN_print_fp(stdout, M);printf("\n");
    unsigned char *to = malloc((BN_num_bytes(M) +1)*sizeof(char));
    BN_bn2bin(M, to);
    // for(int i=0;i<32;i++){
    //     printf("%d ", to[i] );
    // }
    // printf("\n");
    //BN_rand(M,4,0,0);


    // generating sigma of 3 bit
    BIGNUM *sigma = BN_new();
    BN_rand(sigma,3,0,0);
    //calculating r_m
    BIGNUM *r_m = BN_new();
    char *sol;
    sol = take_Concatenate_new(pol,M,sigma);
    DEBUG(__LINE__);
    // printf("%s \n", sol);
    //computing hash of sol for r_m
    compute_hash_new(r_m,sol);
    BN_mod(r_m,r_m,pub->p,ctx);
    // DEBUG(__LINE__);
    // BN_print_fp(stdout, r_m); printf("\n");

    // calculating polynomial of f(x,policy)

    unsigned long long int *polynomial = coefficients(attributes,pub->n);

    // for(int i=0;i<=pub->n;i++) printf("%d ", polynomial[i]);
    // printf("\n");


    EC_POINT *k1m = EC_POINT_new(curve);
    EC_POINT *k2m = EC_POINT_new(curve);
    EC_POINT *temp_pt = EC_POINT_new(curve);
    //
    if(EC_POINT_set_to_infinity(curve,k1m)!=1){
        DEBUG(__LINE__);
        printf("Unable to set at infinity 1\n");
    }
    if(EC_POINT_set_to_infinity(curve,k2m)!=1){
        DEBUG(__LINE__);
        printf("Unable to set at infinity 2\n");
    }
    //
    BIGNUM *x1 = BN_new();
    BIGNUM *y1 = BN_new();
    BIGNUM *fi = BN_new();

    // printf("Inside Loop 1: \n");
    for(int i=0;i<=pub->n;i++) {
        BN_set_word(fi,polynomial[i]);
        EC_POINT_mul(curve, temp_pt, NULL, pub->U_i[i], fi, ctx);
        EC_POINT_add(curve, k1m,   k1m,  temp_pt,   ctx);
    }
    // printf("Inside Loop 2: \n");
    for(int i=0;i<=pub->n;i++) {
        BN_set_word(fi,polynomial[i]);
        // printf(" i = %d fi = ", i); 
        // BN_print_fp(stdout, fi); printf("\n");
        EC_POINT_mul(curve,temp_pt,NULL,pub->V_i[i],fi,ctx);
        EC_POINT_add(curve,k2m,k2m,temp_pt,ctx);
    }

    EC_POINT_mul(curve,k1m,NULL,k1m, r_m,ctx);
    EC_POINT_mul(curve,k2m,NULL,k2m, r_m,ctx);
        // EC_POINT_get_affine_coordinates(curve, temp_pt, x1, y1, NULL);
        // BN_print_fp(stdout, x1);
        // putc('\n', stdout);
        // BN_print_fp(stdout, y1);
        // putc('\n', stdout);


    //calculating f(alpha)
    BIGNUM *f_alpha = BN_new();
    BN_zero(f_alpha);
    f(msk->alpha,attributes,pub->n);



    // rmp

    EC_POINT *r_m_P = EC_POINT_new(curve);
    if (1 != EC_POINT_mul(curve, r_m_P, r_m, NULL, NULL, ctx)) 
        handleErrors(__LINE__);
    
    // getting coordinates of r_m_P


    BIGNUM *x = BN_new();
    BIGNUM *y = BN_new();
    EC_POINT_get_affine_coordinates_GFp(curve,r_m_P,x,y,ctx);

    //
    char *pass1,*pass2;
    pass1=(char*)BN_bn2dec(x);
    pass2=(char*)BN_bn2dec(y);
    int B = 2;    
    unsigned char *pass = (unsigned char*)calloc(200,sizeof(unsigned char)) ;
    unsigned char *out = (unsigned char*)calloc(B,sizeof(unsigned char));
    strcpy(pass,"(");
    strcat(pass,pass1);
    strcat(pass,",");
    strcat(pass,pass2);
    strcat(pass,")");
    //using kdf function
    PKCS5_PBKDF2_HMAC_SHA1(pass,strlen(pass),NULL,0,1000,B,out);

    // calculating K_m
    int idx_out = 0,x_out;
    int temp_out[2*B];
    
    for(int i =0;i<B;i++){
      x_out = (int)out[i];
      temp_out[idx_out++] = x_out/16;
      temp_out[idx_out++] = x_out%16;
    }
    BIGNUM *K_m = BN_new();
    BN_zero(K_m);
    for(int i=0;i<2*B;i++){
      BN_mul_word(K_m,16);
      BN_add_word(K_m,temp_out[i]);
    }
    //calculating k1m and k2m
    // EC_POINT *K1m = EC_POINT_new(curve);
    // EC_POINT *K2m = EC_POINT_new(curve);

    // EC_POINT *temp = EC_POINT_new(curve);
    // EC_POINT_mul(curve,temp,NULL,r_m_P,f_alpha,ctx);

    // EC_POINT_mul(curve,K1m,NULL,temp,k1,ctx);
    // EC_POINT_mul(curve,K2m,NULL,temp,k2,ctx);

    //computing cypher text

    bswabe_cph_t * cph  = (bswabe_cph_t*)malloc(sizeof(bswabe_cph_t));

    BIGNUM *C_sigma = BN_new();
    BIGNUM *C_m = BN_new();

    BIGNUM *H2_km = BN_new();
    BIGNUM *H3_sigma = BN_new();

    char *temp2 = BN_bn2dec(K_m);

    compute_hash_new(H2_km,temp2);
    compute_hash2_new(H3_sigma,sigma);

    BN_GF2m_add(C_sigma,H2_km,sigma);
    BN_GF2m_add(C_m,H3_sigma,M);
    // seting values of   k1m , k2m , policy , c_sigma_m and c_m in cipher text
    cph->C_m = C_m;
    cph->C_sigma_m = C_sigma;
    cph->K_1m = k1m;
    cph->K_2m = k2m;

    for (int i=0;i<pub->n;i++) {
        cph->Policy[i] = attributes[i];
    }
    // calculating Pmi
    int ones = 0;                        // counting set bits in policy
    for(int i=0;i<pub->n;i++){
        if(attributes[i]==1)
            ++ones;
    }

    //computing P_m_i

    for(int i=1;i<=pub->n - ones;i++) {
      cph->P_m_i[i] = EC_POINT_new(curve);
      EC_POINT_mul(curve,cph->P_m_i[i],NULL,pub->P_i[i],r_m,ctx);
    }
    BN_CTX_free(ctx);
    return cph;
}



bswabe_prv_t *bswabe_keygen(bswabe_prv_t** prv, bswabe_pub_t *pub, bswabe_msk_t *msk, int user_attr_set[])
{
    // DEBUG(__LINE__);
    BN_CTX *ctx ;
    if (NULL == (ctx = BN_CTX_new()))
        printf("error\n");
    //DEBUG(__LINE__);
    *prv = malloc(sizeof(bswabe_prv_t));
    //DEBUG(__LINE__);
    (*prv)->u1 = BN_new();
    (*prv)->u2 = BN_new();

    // //DEBUG(__LINE__);
    BIGNUM *f_alpha = f(msk->alpha, user_attr_set,pub->n); // findinf f(alpha, attributes)
    BIGNUM *s_u = BN_new();
    BIGNUM *t_u = BN_new();
    BIGNUM *r_u = BN_new();

    // Initialise r_u and t_u with random values 
    BN_rand_range(r_u, pub->p); 
    BN_rand_range(t_u, pub->p);

    //DEBUG(__LINE__);
    BIGNUM *k1_inv = BN_new();
    BIGNUM *f_inv = BN_new();

    // Evaluating s_u
    BN_mod_inverse(k1_inv, msk->k1, pub->p, ctx); // k1_inv = (1/k1) mod p
    BN_mod_inverse(f_inv, f_alpha, pub->p, ctx); // f_inv = (1/f_alpha) mod p
    BN_mod_mul(s_u, msk->k2, r_u, pub->p, ctx); // s_u = k2 * r_u
    BN_mod_sub(s_u,f_inv,s_u,pub->p,ctx); // s_u = (1/f_alpha) - k2* r_u
    BN_mod_mul(s_u, s_u, k1_inv, pub->p, ctx); // s_u = (1/k1) * s_u

    // Finding u1
    //DEBUG(__LINE__);
    BN_mod_mul((*prv)->u1, msk->k1, t_u, pub->p, ctx); // u1 = k1 * t_u
	BN_print_fp(stdout, (*prv)->u1); printf("\n");
    // BN_mod_add(prv->u1, r_u, prv->u1, pub->p, ctx); // u1 = r_u + k1*t_u (mod p)

    //DEBUG(__LINE__);
    // Finding u2
    BN_mod_mul((*prv)->u2, msk->k2, t_u, pub->p, ctx); // u2 = k2* t_u
    //DEBUG(__LINE__);
    BN_mod_sub((*prv)->u2, s_u, (*prv)->u2, pub->p, ctx); // u2 = s_u - k2*t_u (mod p)

    //DEBUG(__LINE__);
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

int bswabe_dec(bswabe_pub_t* pub, bswabe_prv_t* prv, bswabe_cph_t* cph, char* m) // attrib is P
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
    printf("hello i am  == %s", str1);
    // compute_hash(H2_K_m, str1);

    mpz_xor(sigma_d_m, H2_K_m, cph->C_sigma);
    gmp_printf("value of sigma_d_m = %Zd", sigma_d_m);
    mpz_t H3_sigma_m;
    mpz_init(H3_sigma_m);

    //take_Concatenate(str2,sigma_d_m,blank1,blank2);
    int str2 = mpz_get_ui(sigma_d_m);
    // compute_hash2(H3_sigma_m, str2);

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
    // compute_hash2(H1, final_ans);

    if (mpz_cmp(cph->S_m, H1) == 0)
    {
        return 1;
    }

    return 0;
}
