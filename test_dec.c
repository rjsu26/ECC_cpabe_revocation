#include <openssl/bn.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
// #include <glib.h>
#include <pbc.h>
#include <pbc_random.h>
#include <time.h>

#include "bswabe.h"
#include "common.h"

unsigned long long int *coefficients_dec(int attribute[], int policy[],int n) 
{
  unsigned long long int *poly = (unsigned long long int*)calloc(n+1,sizeof(unsigned long long int));
  for(int i=0;i<=n;i++) 
  {
    poly[i] = 0;
  }
  poly[0]=1;
  int set_bit_count=0;
  for(int i=0;i<n;i++)
  {
	  if(policy[i]==1)
	  {
		  set_bit_count++;
	  }
  }
  for(int i=0; i<(n-set_bit_count) ;i++) 
  {
    if(attribute[i]-policy[i]==1)
	{
      multi(poly,n,hash_4(i+1));
    }
  }

  return poly;
}

void multi(unsigned long long int *arr,int n,int a) 
{
  //mul with x
  unsigned long long int *temp = (unsigned long long int*)calloc(n+1,sizeof(unsigned long long int));
  for(int i=n;i>=0;i--)
  {
    temp[i] = arr[i]*a;
  }
  for(int i=n-1;i>=0;i--) 
  {
    arr[i+1] = arr[i];
  }
  arr[0] = 0;
  for(int i=0;i<=n;i++) 
  {
    arr[i] = arr[i] + temp[i];
  }
  free(temp);
}

int bswabe_dec(bswabe_pub_t *pub, bswabe_prv_t *prv, bswabe_cph_t *cph, char* m, int attributes[]) // attrib is P
{


/***********************************
	      D1 = DONE
************************************/
	for(int i=0;i<pub->n;i++)
	{
		if(cph->Policy[i]!=attribute[i]) 
		{
			return 0;
		}	
	}
	

/***********************************
	      D2  = DONE
************************************/

	EC_GROUP *curve = create_curve(pub->a,pub->b,pub->p,pub->order,pub->G_x,pub->G_y);

	// Create two points U and V on ECC 
	BN_CTX *ctx = BN_CTX_new();
	EC_POINT *U = EC_POINT_new(curve);
	EC_POINT *V = EC_POINT_new(curve);

	// Calculation of U and V
	EC_POINT_mul(curve, U, NULL, prv->u2, cph->K_1m, ctx);
	EC_POINT_mul(curve, V, NULL, prv->u1, cph->K_2m, ctx);
	
	// Calculating U+V and storing in new variable 'sum_of_U_and_V'

	EC_POINT *sum_of_U_and_V = EC_POINT_new(curve);
	EC_POINT_add(curve, sum_of_U_and_V,   U,  V,   ctx);

	
/***********************************
	      //D3 = DONE
************************************/
	
	EC_POINT *W = EC_POINT_new(curve);

	unsigned long long int *polynomial = coefficients_dec(attributes,pub->n);
	
	int count=0;

	for(int i=0;i< pub->n;i++)
	{
		if(prv->policy[i]==0) count++;
	}
	
	EC_POINT *temp_sum = EC_POINT_new(curve);

	for(int i=0; i<(pub->n-count) ;i++)
	{
		//ECC MULTIPLICATION
        EC_POINT_mul(curve,temp_sum,NULL,polynomial[i], cph->P_m_i[i],ctx);
		EC_POINT_add(curve,W,W,temp_sum,ctx);
	}

	EC_POINT_sub(curve,sum_of_U_and_V,sum_of_U_and_V,W,ctx); //if Subtraction exists??

	EC_POINT *r_m_P = EC_POINT_new(curve);

	EC_POINT_div(curve,r_m_P, sum_of_U_and_V, F_0,ctx);	//if Division exists???



/***********************************
	      //D4 = DONE
************************************/



    BIGNUM *x = BN_new();
    BIGNUM *y = BN_new();
    EC_POINT_get_affine_coordinates_GFp(curve,r_m_P,x,y,ctx);

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

    PKCS5_PBKDF2_HMAC_SHA1(pass,strlen(pass),NULL,0,1000,B,out);
 
	BIGNUM *sigma_m_new=BN_new();
	BIGNUM *M_new=BN_new();

	BIGNUM *H2_kdf_r_m_P=BN_new();
	BIGNUM *H3_sigma=BN_new();

	compute_hash(H2_kdf_r_m_P, out);
	BN_GF2m_add(sigma_m_new,H2_kdf_r_m_P, cph->C_sigma_m);

	compute_hash2(H3_sigma,sigma_m_new);
    BN_GF2m_add(M_new,cph->C_m,H3_sigma);

	BIGNUM *r_m_new=BN_new();
	char *sol;
    sol = take_Concatenate(pol,M,sigma);
    compute_hash(r_m_new,sol);
    BN_mod(r_m_new,r_m_new,pub->p,ctx);

	EC_POINT *r_m_P_new = EC_POINT_new(curve);

    EC_POINT_mul(curve,r_m_P_new,r_m_new, NULL, NULL,ctx);

    char *M = BN_bn2dec(M_new);

	if(r_m_P_new == r_m_P) // replace with comparison function
	{
		m = M;
		return 1;
	}

    BN_CTX_free(ctx);
	return 0;
}