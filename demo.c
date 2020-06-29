// Sample program to try and test all features before putting it into the main program files. 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <gmp.h>
#include <assert.h>
#include <time.h>

#define DEBUG(arg) printf("file %s Line #%d\n", __FILE__, arg);

struct trial{
    mpz_t b;
    // mpz_t a;
    // mpz_t c;
    // trial(){
    //     arr[3]= {1,2};
    // }
};

typedef struct trial demo;

// void setup(demo **mpk, int n){
//     (*mpk)->num = n;
//     int temp[]= {2,76,45};
//     unsigned char d[2] ={0x13,0x7a}; 
//     // (*mpk)->arr =temp;
//     memcpy((*mpk)->arr, temp, sizeof(temp));
//     memcpy((*mpk)->b, d, sizeof(d));
//     // strcpy((*mpk)->arr,temp);
//     return ;
// }

int main(){
    demo *mpk = (demo*) malloc(sizeof(demo));
    mpz_init_set_ui(mpk->b,1);
    DEBUG(__LINE__);
    // mpz_init(pie);
    // mpz_set_ui(pie, 0);
    mpz_t p,rand;
    mpz_init(rand);
    int flag =  mpz_init_set_str (p, "115792089237316195423570985008687907853269984665640564039457584007908834671661", 10);
    assert(flag==0);
    // printf("p = ");
    unsigned long seed;
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    // srand(time(NULL));
    seed = (int) time(NULL);
    gmp_randseed_ui(state, seed);
    mpz_out_str(stdout,10, mpk->b);
    printf ("\n");
    // mpz_class ran;
    for(int i=0;i<10;i++){
        // DEBUG(__LINE__);
        mpz_urandomm(mpk->b, state, p);
        // assert(flag==0);
        mpz_out_str(stdout,10, mpk->b);
        printf ("\n");

    }

    // setup(&mpk, 2);
    // for(int i=0;i<2;i++)
    //     printf("%x ",  mpk->b[i]);
    return 0;
}