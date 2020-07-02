#include <stdio.h>
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <string.h>
// #include "./libbswabe-0.9/private.h"
// #include "./libbswabe-0.9/bswabe.h"
//imp lib

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

void take_Concatenate(char sol[], BIGNUM *m1, BIGNUM *m2, BIGNUM *m3)
{

    char *s1;
    char *s2;
    char *s3;

    int flag = 0;
    int flag2 = 0;
    int flag3 = 0;
    if (BN_is_zero(m1) == 1)
    {
      //printf("i am here\n");
        s1 = "A";
    }
    else
    {
        s1 = BN_bn2dec(m1);
        //printf(" s1  ->  %s",s1);
        flag = strlen(s1);
    }

    if (BN_is_zero(m2) == 1)
    {
        s2 = "A";
    }
    else
    {
        s2 = BN_bn2dec(m2);
        //printf(" s2  ->  %s",s2);
        flag2 = strlen(s2);
    }

    if (BN_is_zero(m3) == 1)
    {
        s3 = "A";
    }
    else
    {
        s3 = BN_bn2dec(m3);
       // printf(" s3  ->  %s",s3);
        flag3 = strlen(s3);
    }

    int size = flag + flag2 + flag3;
    char ans[size];
    concatenate(ans, s1, s2, s3);   // concatenate 
    int i;
    for (i = 0; i < size; i++)
    {
        sol[i] = ans[i];
    }
}

void compute_hash(BIGNUM *hash, char *str)
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
void compute_hash2(BIGNUM *hash, BIGNUM *x)
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


BIGNUM *f(BIGNUM *x,int *attributes,int n) {
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

EC_GROUP *create_curve(void)
{
    BN_CTX *ctx;
    EC_GROUP *curve;
    BIGNUM *a, *b, *p, *order, *x, *y;
    EC_POINT *generator;

    
    int BITS = 32;

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
    unsigned char p_bin[32] =       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFC, 0x2F};
    unsigned char order_bin[32] =   {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xBA, 0xAE, 0xDC, 0xE6, 0xAF, 0x48, 0xA0, 0x3B, 0xBF, 0xD2, 0x5E, 0x8C, 0xD0, 0x36, 0x41, 0x41};
    unsigned char x_bin[32] =       {0x79, 0xbe, 0x66, 0x7e, 0xf9, 0xdc, 0xbb, 0xac, 0x55, 0xa0, 0x62, 0x95, 0xce, 0x87, 0x0b, 0x07, 0x02, 0x9b, 0xfc, 0xdb, 0x2d, 0xce, 0x28, 0xd9, 0x59, 0xf2, 0x81, 0x5b, 0x16, 0xf8, 0x17, 0x98};
    unsigned char y_bin[32] =       {0x48, 0x3a, 0xda, 0x77, 0x26, 0xa3, 0xc4, 0x65, 0x5d, 0xa4, 0xfb, 0xfc, 0x0e, 0x11, 0x08, 0xa8, 0xfd, 0x17, 0xb4, 0x48, 0xa6, 0x85, 0x54, 0x19, 0x9c, 0x47, 0xd0, 0x8f, 0xfb, 0x10, 0xd4, 0xb8};
    
    /* Set up the BN_CTX */
    if(NULL == (ctx = BN_CTX_new())) handleErrors(__LINE__);

    /* Set the values for the various parameters */
    if(NULL == (a = BN_bin2bn(a_bin, BITS, NULL))) handleErrors(__LINE__);
    if(NULL == (b = BN_bin2bn(b_bin, BITS, NULL))) handleErrors(__LINE__);
    if(NULL == (p = BN_bin2bn(p_bin, BITS, NULL))) handleErrors(__LINE__);
    if(NULL == (order = BN_bin2bn(order_bin, BITS, NULL))) handleErrors(__LINE__);
    if(NULL == (x = BN_bin2bn(x_bin, BITS, NULL))) handleErrors(__LINE__);
    if(NULL == (y = BN_bin2bn(y_bin, BITS, NULL))) handleErrors(__LINE__);

    /* Create the curve */
    if(NULL == (curve = EC_GROUP_new_curve_GFp(p, a, b, ctx))) handleErrors(__LINE__);

    /* Create the generator */
    if(NULL == (generator = EC_POINT_new(curve))) handleErrors(__LINE__);
    if(1 != EC_POINT_set_affine_coordinates_GFp(curve, generator, x, y, ctx))
        handleErrors(__LINE__);

    /* Set the generator and the order */
    if(1 != EC_GROUP_set_generator(curve, generator, order, NULL))
        handleErrors(__LINE__);

    EC_POINT_free(generator);
    BN_free(y);
    BN_free(x);
    BN_free(order);
    BN_free(p);
    BN_free(b);
    BN_free(a);
    BN_CTX_free(ctx); 

    return curve;
}





//

void test(void){
  BN_CTX *ctx = BN_CTX_new();
  int n = 4;
  int attributes[4] = {1,0,1,0};
  BIGNUM *pol = BN_new();
  BN_zero(pol);
  for(int i=0;i<n;i++) {
    BN_mul_word(pol,10);
    BN_add_word(pol,attributes[i]);
  } 
  BIGNUM *M = BN_new();
  BN_rand(M,30,-1,0);
  BIGNUM *sigma = BN_new();
  BN_rand(sigma,30,-1,0);
  BIGNUM *r_m = BN_new();
  char sol[100];
  take_Concatenate(sol,pol,M,sigma);
  printf("concatenated string is %s\n",sol);
  //BIGNUM *hash = BN_new();
  compute_hash(r_m,sol);
  print_bignum(r_m);
  BIGNUM *a = BN_new();
  BN_set_word(a,2);
  BIGNUM *f_alpha =  f(a,attributes,n);  
  print_bignum(f_alpha);


  //

  EC_GROUP *curve = create_curve();
  //generate the private key
    BIGNUM *prv = BN_new();
    unsigned char prv_bin[2] = {0x00, 0x01};
    
    prv = BN_bin2bn(prv_bin, 2, NULL);


    //declare empty public key
    EC_POINT *pub = EC_POINT_new(curve);
    //print_point(curve,pub,ctx);
    /* get public key values from private key*/
    if (1 != EC_POINT_mul(curve, pub, prv, NULL, NULL, ctx)) 
        handleErrors(__LINE__);
    //print_point(curve,pub,ctx);
    EC_POINT *r_m_P = EC_POINT_new(curve);
    if (1 != EC_POINT_mul(curve, r_m_P, r_m, NULL, NULL, ctx)) 
        handleErrors(__LINE__);
    printf("RmP\n");
    print_point(curve,r_m_P,ctx);
    BIGNUM *x = BN_new();
    BIGNUM *y = BN_new();
    EC_POINT_get_affine_coordinates_GFp(curve,r_m_P,x,y,ctx);
    print_bignum(x);
    print_bignum(y);
    char *pass1,*pass2;
    pass1=(char*)BN_bn2dec(x);
    pass2=(char*)BN_bn2dec(y);
    char pass[200] ;
    //unsigned char out[200];
    strcpy(pass,"(");
    strcat(pass,pass1);
    strcat(pass,",");
    strcat(pass,pass2);
    strcat(pass,")");
    //printf("%s\n",pass);
    // const EVP_MD *digest = EVP_sha1();
    // PKCS5_PBKDF2_HMAC(pass,-1,NULL,0,1000,digest,4,out);
    // printf("%s\n",out);
    BIGNUM *K_m = BN_new();
    BN_rand(K_m,32,-1,0);

    BIGNUM *k1 = BN_new();
    BIGNUM *k2 = BN_new();
    BIGNUM *p = BN_new();
    unsigned char p_bin[32] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFC, 0x2F};
    p = BN_bin2bn(p_bin,32,NULL);
    BN_rand_range(k1,p);
    BN_rand_range(k2,p);
  //
    
    print_bignum(p);
    print_bignum(k1);
    print_bignum(k2);

    EC_POINT *K1m = EC_POINT_new(curve);
    EC_POINT *K2m = EC_POINT_new(curve);

    EC_POINT *temp = EC_POINT_new(curve);
    EC_POINT_mul(curve,temp,NULL,r_m_P,f_alpha,ctx);

    EC_POINT_mul(curve,K1m,NULL,temp,k1,ctx);
    EC_POINT_mul(curve,K2m,NULL,temp,k2,ctx);

    print_point(curve,K1m,ctx);
    print_point(curve,K2m,ctx);


    BIGNUM *C_sigma = BN_new();
    BIGNUM *C_m = BN_new();

    BIGNUM *H2_km = BN_new();
    BIGNUM *H3_km = BN_new();

    char *temp2 = BN_bn2dec(K_m);

    compute_hash(H2_km,temp2);
    compute_hash2(H3_km,sigma);
    print_bignum(H2_km);
    print_bignum(H3_km);

    BN_GF2m_add(C_sigma,K_m,sigma);
    BN_GF2m_add(C_m,sigma,M);
    print_bignum(C_sigma);
    print_bignum(C_m);
    

    EC_POINT *P_m_i[100],*P_i[100];
    P_i[0] = EC_POINT_new(curve);
    EC_POINT *P = EC_POINT_new(curve);
    P = EC_GROUP_get0_generator(curve);
    print_point(curve,P,ctx);
    EC_POINT_copy(P_i[0],P);
    BIGNUM *a_i =BN_new();
    BIGNUM *idx = BN_new();
    for(int i=1;i<=n;i++) {
      BN_set_word(idx,i);
      print_bignum(idx);
      BN_exp(a_i,a,idx,ctx);
      print_bignum(a_i);
      P_i[i] = EC_POINT_new(curve);
      EC_POINT_mul(curve,P_i[i],NULL,P,a_i,ctx);
      P_m_i[i] = EC_POINT_new(curve);
      EC_POINT_mul(curve,P_m_i[i],NULL,P_i[i],r_m,ctx);
      print_point(curve,P_m_i[i],ctx);
    }

    BN_CTX_free(ctx);

}



int main() {
    //  BIGNUM *a = BN_new();
    //  BN_set_word(a,64);
    //  BIGNUM *b = a;
    //  print_bignum(b);
  //   //BIGNUM *r = BN_new();
  //  //ans[0] = ans[1] = ans[2] = 'a';
  //    BN_set_word(a,61);
  //   // BN_set_word(b,3);
  //   // int size = BN_num_bytes(b);
  //   // unsigned char *ans = malloc((size+1)*(sizeof(unsigned char)));
  //   //ans[size] = '\0';
  //   //BN_rand(a,1,0,0);
  //   //char *to = binOfbn(b);
  //   //BIGNUM *b = BN_new();
  //   //BN_set_word(b,BN_get_word(a)); // copy one bn to bn
  //   char *to ;//= BN_bn2dec(a); // copy karne ke liye
  //   // BN_GF2m_add(r,a,b);       //XOR
  //   // //int len = BN_bn2bin(b,ans);
  //   // //printf("hello world"); 
  //   // //printf("%s\n",to);

  //   // char *A,*B,*R;
  //   // A = binOfbn(a);  // bn to bin char (printable)
  //   // B = binOfbn(b);
  //   // R = binOfbn(r);
  //   to[0] = 'e';
  //   int size = strlen(to);
  //   printf("%d\n%s\n",size,to);
    // for(int i=0;to[i]!='\0';i++){
    //     printf("%c ",to[i]);
    // }
     //printf("a - %s\n",to);
    // printf("b - %s\n",B);
    // printf("r - %s\n",R);
    //print_bignum(b);
    //printf("%d\n",size);

    test();
}