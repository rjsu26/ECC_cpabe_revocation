#include <stdio.h>
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <string.h>

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
void print_bignum(BIGNUM * num) {
    BN_print_fp(stdout, num);
    fprintf(stdout, "\n");
}

void handleErrors(int lineNumber) {
    printf("Error at LINE=%d\n", lineNumber);
}

//MSk has alpha k1 and k2

void test(void){
    char *ans;
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *alpha = BN_new();
    BIGNUM *k1 = BN_new();
    BIGNUM *k2 = BN_new();
    BIGNUM *mod = BN_new();
    BN_rand(alpha,32,-1,0);
    ans = BN_bn2dec(alpha);
    printf("alpha -> %s\n",ans);
    BN_rand(k1,32,-1,0);
    ans = BN_bn2dec(k1);
    printf("k1 -> %s\n",ans);
    BN_rand(k2,32,-1,0);
    ans = BN_bn2dec(k2);
    printf("k2 -> %s\n",ans);
    unsigned char p_bin[32] =       {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFC, 0x2F};
    mod = BN_bin2bn(p_bin,32,NULL);
    ans = BN_bn2dec(mod);
    printf("mod -> %s\n",ans);
    int n = 4; 
    int a_i[4] = {1,0,1,1};
    BIGNUM *f_alpha = f(alpha,a_i,n);
    ans = BN_bn2dec(f_alpha);
    printf("f_alpha -> %s\n",ans);
    BIGNUM *s_u = BN_new();
    BIGNUM *t_u = BN_new();
    BIGNUM *r_u = BN_new();
    BN_rand(t_u,32,-1,0);
    ans = BN_bn2dec(t_u);
    printf("t_u -> %s\n",ans);
    BN_rand(r_u,32,-1,0);
    ans = BN_bn2dec(r_u);
    printf("r_u -> %s\n",ans);
    BIGNUM *k1_inv = BN_new();
    BIGNUM *f_inv = BN_new();
    //BIGNUM *k2_ru = BN_new();
    k1_inv = BN_mod_inverse(NULL,k1,mod,ctx);
    ans = BN_bn2dec(k1_inv);
    printf("k1_inv -> %s\n",ans);
    f_inv = BN_mod_inverse(NULL,f_alpha,mod,ctx);
    ans = BN_bn2dec(f_inv);
    printf("f_inv -> %s\n",ans);
    BN_mod_mul(s_u,k2,r_u,mod,ctx);
    BN_mod_sub(s_u,f_inv,s_u,mod,ctx);
    BN_mod_mul(s_u,s_u,k1_inv,mod,ctx);
    ans = BN_bn2dec(s_u);
    printf("s_u -> %s\n",ans);
    //print_bignum(s_u);
    //ans = BN_bn2dec(s_u);
    BIGNUM *u1 = BN_new();
    BIGNUM *u2 = BN_new();
    BN_mod_mul(u1,k1,t_u,mod,ctx);
    BN_mod_add(u1,r_u,u1,mod,ctx);
    BN_mod_mul(u2,k2,t_u,mod,ctx);
    BN_mod_sub(u2,s_u,u2,mod,ctx);
    ans = BN_bn2dec(u1);
    printf("u1 -> %s\n",ans);
    ans = BN_bn2dec(u2);
    printf("u2 -> %s\n",ans);
    BN_CTX_free(ctx);
}
int main(){
    test();
}