/*
  Include glib.h and pbc.h before including this file. Note that this
  file should be included at most once.
*/
#include <openssl/ec.h>
#include <openssl/bn.h>


#if defined (__cplusplus)
extern "C" {
#endif

/*
  A public key.
*/
typedef struct bswabe_pub_s bswabe_pub_t;

/*
  A master secret key.
*/
typedef struct bswabe_msk_s bswabe_msk_t;
/*
 proxy element
*/
typedef struct bswabe_proxy_s bswabe_proxy_t;
/*
  A private key.
*/
typedef struct bswabe_prv_s bswabe_prv_t;

/*
  A ciphertext. Note that this library only handles encrypting a
  single group element, so if you want to encrypt something bigger,
  you will have to use that group element as a symmetric key for
  hybrid encryption (which you do yourself).
*/
typedef struct bswabe_cph_s bswabe_cph_t;

typedef struct bswabe_verification_s bswabe_verification_t;

typedef struct bswabe_sig_s bswabe_sig_t;

typedef struct bswabe_signature_s bswabe_signature_t;

typedef struct bswabe_sigver_s bswabe_sigver_t;

/*
  Generate a public key and corresponding master secret key, and
  assign the *pub and *msk pointers to them. The space used may be
  later freed by calling bswabe_pub_free(*pub) and
  bswabe_msk_free(*msk).
*/
void bswabe_setup( bswabe_pub_t** pub, bswabe_msk_t** msk ,int n);

/*
  Generate a private key with the given set of attributes. The final
  argument should be a null terminated array of pointers to strings,
  one for each attribute.
*/
bswabe_prv_t* bswabe_keygen( bswabe_prv_t** prv,bswabe_pub_t* pub,
                             bswabe_msk_t* msk,
                              int user_attr_set[]);

void compute_hash2(mpz_t hash,unsigned long int x);

void compute_hash(mpz_t hash,char str[]) ;

unsigned long int generate_random(mpz_t rand_Num,int limit);

void take_Concatenate(char sol[], mpz_t m1, mpz_t m2, mpz_t m3);

void concatenate(char ans[],char* s1, char* s2, char* s3);

int isPrime(int x);

/*
  Pick a random group element and encrypt it under the specified
  access policy. The resulting ciphertext is returned and the
  element_t given as an argument (which need not be initialized) is
  set to the random group element.

  After using this function, it is normal to extract the random data
  in m using the pbc functions element_length_in_bytes and
  element_to_bytes and use it as a key for hybrid encryption.

  The policy is specified as a simple string which encodes a postorder
  traversal of threshold tree defining the access policy. As an
  example,

    "foo bar fim 2of3 baf 1of2"

  specifies a policy with two threshold gates and four leaves. It is
  not possible to specify an attribute with whitespace in it (although
  "_" is allowed).

  Numerical attributes and any other fancy stuff are not supported.

  Returns null if an error occured, in which case a description can be
  retrieved by calling bswabe_error().
*/

int *delete_subarr(int arr_primes[],int arr_remove[], int n,int m);
bswabe_cph_t* bswabe_enc( bswabe_pub_t* pub, bswabe_msk_t* msk,element_t m, int attrib[]);

int verify(bswabe_verification_t *ver, bswabe_signature_t *signa, bswabe_cph_t *cp, bswabe_pub_t *pub);

bswabe_signature_t* sign(bswabe_sig_t *sig, bswabe_cph_t *cp, bswabe_pub_t *pub);

/*-------------------------------------------------------------------------*/

char* random_binaryString();

bswabe_sigver_t* sigkeygen(bswabe_pub_t* pub);

/*------------------------------------------------------------------------*/

void bswabe_proxy(mpz_t k1, mpz_t C_attr, mpz_t C_user);
/*
  Decrypt the specified ciphertext using the given private key,
  filling in the provided element m (which need not be initialized)
  with the result.

  Returns true if decryption succeeded, false if this key does not
  satisfy the policy of the ciphertext (in which case m is unaltered).
*/
int
bswabe_dec( bswabe_pub_t* pub, bswabe_prv_t* prv, bswabe_cph_t* cph, element_t m);
/*
  Exactly what it seems.
*/
GByteArray* bswabe_pub_serialize( bswabe_pub_t* pub );
GByteArray* bswabe_msk_serialize( bswabe_msk_t* msk ,int n);
GByteArray* bswabe_prv_serialize( bswabe_prv_t* prv );
GByteArray* bswabe_cph_serialize( bswabe_cph_t* cph );
GByteArray* bswabe_verification_serialize( bswabe_verification_t * ver );

// NEW
GByteArray *bswabe_pub_serialize_new(bswabe_pub_t *pub);
GByteArray * bswabe_msk_serialize_new(bswabe_msk_t *msk);
GByteArray *bswabe_prv_serialize_new(bswabe_prv_t *prv);

/*
  Also exactly what it seems. If free is true, the GByteArray passed
  in will be free'd after it is read.
*/
bswabe_pub_t* bswabe_pub_unserialize( GByteArray* b, int free );
bswabe_msk_t* bswabe_msk_unserialize( GByteArray* b, int free );
bswabe_prv_t* bswabe_prv_unserialize( bswabe_pub_t* pub, GByteArray* b, int free );
bswabe_cph_t* bswabe_cph_unserialize( bswabe_pub_t* pub, GByteArray* b, int free );
bswabe_verification_t* bswabe_verification_unserialize( bswabe_pub_t* pub, GByteArray* b, int free );

// NEW
bswabe_pub_t * bswabe_pub_unserialize_new(GByteArray *b, int free);
bswabe_msk_t *bswabe_msk_unserialize_new(GByteArray *b, int free);
/*
  Again, exactly what it seems.
*/
void bswabe_pub_free( bswabe_pub_t* pub );
void bswabe_msk_free( bswabe_msk_t* msk );
void bswabe_prv_free( bswabe_prv_t* prv );
void bswabe_cph_free( bswabe_cph_t* cph );

/*
  Return a description of the last error that occured. Call this after
  bswabe_enc or bswabe_dec returns 0. The returned string does not
  need to be free'd.
*/
char* bswabe_error();

// function to create a new ECC curve
EC_GROUP *create_curve(BIGNUM* a,BIGNUM* b,BIGNUM* p,BIGNUM* order,BIGNUM* x,BIGNUM* y);
#if defined (__cplusplus)
} // extern "C"
#endif
