/* Based on BoringSSL's cert.c fuzzer */

#ifdef __cplusplus
extern "C" {
#endif

#include <openssl/asn1.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <stdint.h>
#include <stdio.h>

#include <libhfuzz/libhfuzz.h>

int rand_predictable __attribute__((weak));
void RAND_reset_for_fuzzing(void) __attribute__((weak));

int LLVMFuzzerInitialize(int* argc, char*** argv)
{
    rand_predictable = 1;
    return 1;
}

int LLVMFuzzerTestOneInput(const uint8_t* buf, size_t len)
{
    if (RAND_reset_for_fuzzing)
        RAND_reset_for_fuzzing();

    const uint8_t* b = buf;
    X509* x = d2i_X509(NULL, &b, len);
    if (x) {
        BIO* o = BIO_new_fp(stdout, BIO_NOCLOSE);
        X509_print_ex(o, x, XN_FLAG_RFC2253, X509_FLAG_COMPAT);

        unsigned char* der = NULL;
        i2d_X509(x, &der);
        OPENSSL_free(der);

        X509_free(x);
        BIO_free(o);
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
