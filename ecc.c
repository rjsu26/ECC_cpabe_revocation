#include <stdio.h>
#include <openssl/ec.h>
#include <openssl/bn.h>

void handleErrors(int lineNumber); 
void print_bignum(BIGNUM * num);
void print_point(EC_GROUP *curve, EC_POINT *point, BN_CTX *ctx);
EC_GROUP *create_curve(void);


int main() {
    // Define a new curve
    EC_GROUP *curve;
    //Custom curve
    curve = create_curve();

    BN_CTX *ctx = BN_CTX_new();


    //generate the private key
    BIGNUM *prv = BN_new();
    unsigned char prv_bin[2] = {0x00, 0x01};
    
    prv = BN_bin2bn(prv_bin, 2, NULL);


    //declare empty public key
    EC_POINT *pub = EC_POINT_new(curve);

    /* get public key values from private key*/
    if (1 != EC_POINT_mul(curve, pub, prv, NULL, NULL, ctx)) 
        handleErrors(__LINE__);


/////////////////////////
    EC_POINT *r = EC_POINT_new(curve);

    /* get public key values from private key*/
    if (1 != EC_POINT_mul(curve, r, prv, NULL, NULL, ctx)) 
        handleErrors(__LINE__);

    BIGNUM *k = BN_new();
    unsigned char k_bin[2] = {0x00, 0x15};
    
    k = BN_bin2bn(k_bin, 2, NULL);

    int N= 10000;
    printf("Beginning calculation for %d point doublings.\n", N);
    clock_t begin = clock();
    for(int i=1;i<=N;i++)
        if(1 != EC_POINT_dbl(curve, r, pub, ctx))
                handleErrors(__LINE__);

    clock_t end = clock();
    double time_spent = (double)(end-begin);
    printf("Total time taken= %f seconds\n", time_spent/CLOCKS_PER_SEC);
    printf("Time for 1 doubling: %f seconds \n\n", time_spent/N/CLOCKS_PER_SEC);

    printf("Beginning calculation for %d point additions.\n", N);
    begin = clock();
    for(int i=1;i<=N;i++)
        if(!EC_POINT_add(curve, pub, r, pub, ctx))
            handleErrors(__LINE__);

    end = clock();
    time_spent = (double)(end-begin);
    printf("Total time taken= %f seconds\n", time_spent/CLOCKS_PER_SEC);
    printf("Time for 1 addition: %f seconds \n\n", time_spent/N/CLOCKS_PER_SEC);

    
    printf("Beginning calculation for %d point scalar multiplications.\n", N);
    begin = clock();
    for(int i=1;i<=N;i++)
        if (1 != EC_POINT_mul(curve, r, k, NULL, NULL, ctx)) 
            handleErrors(__LINE__);  

    end = clock();
    time_spent = (double)(end-begin);
    printf("Total time taken= %f seconds\n", time_spent/CLOCKS_PER_SEC);
    printf("Time for 1 scalar multiplication: %f seconds \n\n", time_spent/N/CLOCKS_PER_SEC);
//////////////////


    return 0;
}

// CUSTOM CURVE
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

void print_bignum(BIGNUM * num) {
    BN_print_fp(stdout, num);
    fprintf(stdout, "\n");
}

void handleErrors(int lineNumber) {
    printf("Error at LINE=%d\n", lineNumber);
}