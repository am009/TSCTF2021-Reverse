#include <stdint.h>     //for int8_t
#include <stdio.h>
#include <string.h>
#include <wmmintrin.h>

// gcc ./wp-dec.c -maes -msse2 -g -O1 -o wp-dec
// 开了O1之后才会有简单的汇编。
// https://www.cryptool.org/en/cto/aes-step-by-step

static uint8_t  subkeys[22][16] __attribute__ ((aligned (16)))  = {
0x77, 0x65, 0x6c, 0x63, 0x6f, 0x6d, 0x32, 0x74, 0x73, 0x63, 0x74, 0x66, 0x32, 0x30, 0x32, 0x31, // welcom2tsctf2021
0x72, 0x46, 0xab, 0x40, 0x1d, 0x2b, 0x99, 0x34, 0x6e, 0x48, 0xed, 0x52, 0x5c, 0x78, 0xdf, 0x63,
0xcc, 0xd8, 0x50, 0x0a, 0xd1, 0xf3, 0xc9, 0x3e, 0xbf, 0xbb, 0x24, 0x6c, 0xe3, 0xc3, 0xfb, 0x0f,
0xe6, 0xd7, 0x26, 0x1b, 0x37, 0x24, 0xef, 0x25, 0x88, 0x9f, 0xcb, 0x49, 0x6b, 0x5c, 0x30, 0x46,
0xa4, 0xd3, 0x7c, 0x64, 0x93, 0xf7, 0x93, 0x41, 0x1b, 0x68, 0x58, 0x08, 0x70, 0x34, 0x68, 0x4e,
0xac, 0x96, 0x53, 0x35, 0x3f, 0x61, 0xc0, 0x74, 0x24, 0x09, 0x98, 0x7c, 0x54, 0x3d, 0xf0, 0x32,
0xab, 0x1a, 0x70, 0x15, 0x94, 0x7b, 0xb0, 0x61, 0xb0, 0x72, 0x28, 0x1d, 0xe4, 0x4f, 0xd8, 0x2f,
0x6f, 0x7b, 0x65, 0x7c, 0xfb, 0x00, 0xd5, 0x1d, 0x4b, 0x72, 0xfd, 0x00, 0xaf, 0x3d, 0x25, 0x2f,
0xc8, 0x44, 0x70, 0x05, 0x33, 0x44, 0xa5, 0x18, 0x78, 0x36, 0x58, 0x18, 0xd7, 0x0b, 0x7d, 0x37,
0xf8, 0xbb, 0xea, 0x0b, 0xcb, 0xff, 0x4f, 0x13, 0xb3, 0xc9, 0x17, 0x0b, 0x64, 0xc2, 0x6a, 0x3c,
0xeb, 0xb9, 0x01, 0x48, 0x20, 0x46, 0x4e, 0x5b, 0x93, 0x8f, 0x59, 0x50, 0xf7, 0x4d, 0x33, 0x6c,
// 11
};

uint8_t plain[16] __attribute__ ((aligned (16)));
uint8_t computed[16] __attribute__ ((aligned (16))) = {0x55, 0xE2, 0xD0, 0x4A, 0x91, 0xAB, 0x5B, 0x89, 0x86, 0xD8, 0xE2, 0x83, 0x1F, 0x51, 0x8F, 0x88,} ;

void aes_enc(uint8_t plain[]) {
    __m128i m = _mm_loadu_si128((const __m128i*) plain);
    m = _mm_xor_si128(m, *(__m128i*)subkeys[0]);
    m = _mm_aesenc_si128(m, *(__m128i*)subkeys[1]);
    m = _mm_aesenc_si128(m, *(__m128i*)subkeys[2]);
    m = _mm_aesenc_si128(m, *(__m128i*)subkeys[3]);
    m = _mm_aesenc_si128(m, *(__m128i*)subkeys[4]);
    // m = _mm_aesenc_si128(m, *(__m128i*)subkeys[5]);
    m = _mm_aesenc_si128(m, *(__m128i*)subkeys[6]);
    m = _mm_aesenc_si128(m, *(__m128i*)subkeys[7]);
    m = _mm_aesenc_si128(m, *(__m128i*)subkeys[8]);
    m = _mm_aesenc_si128(m, *(__m128i*)subkeys[9]);
    m = _mm_aesenclast_si128(m, *(__m128i*)subkeys[10]);
    _mm_storeu_si128((__m128i_u *)computed, m);
}

void aes_dec(uint8_t ciper[]) {
    __m128i m = _mm_loadu_si128((const __m128i*) ciper);
    m = _mm_xor_si128(m, *(__m128i*)subkeys[10]);
    m = _mm_aesdec_si128(m, *(__m128i*)subkeys[11]);
    m = _mm_aesdec_si128(m, *(__m128i*)subkeys[12]);
    m = _mm_aesdec_si128(m, *(__m128i*)subkeys[13]);
    m = _mm_aesdec_si128(m, *(__m128i*)subkeys[14]);
    // m = _mm_aesdec_si128(m, *(__m128i*)subkeys[15]);
    m = _mm_aesdec_si128(m, *(__m128i*)subkeys[16]);
    m = _mm_aesdec_si128(m, *(__m128i*)subkeys[17]);
    m = _mm_aesdec_si128(m, *(__m128i*)subkeys[18]);
    m = _mm_aesdec_si128(m, *(__m128i*)subkeys[19]);
    m = _mm_aesdeclast_si128(m, *(__m128i*)subkeys[0]);
    _mm_storeu_si128((__m128i_u *)plain, m);
}

void dec_init() {
    _mm_storeu_si128((__m128i_u *)subkeys[11], _mm_aesimc_si128(*(__m128i*)subkeys[9]));
    _mm_storeu_si128((__m128i_u *)subkeys[12], _mm_aesimc_si128(*(__m128i*)subkeys[8]));
    _mm_storeu_si128((__m128i_u *)subkeys[13], _mm_aesimc_si128(*(__m128i*)subkeys[7]));
    _mm_storeu_si128((__m128i_u *)subkeys[14], _mm_aesimc_si128(*(__m128i*)subkeys[6]));
    _mm_storeu_si128((__m128i_u *)subkeys[15], _mm_aesimc_si128(*(__m128i*)subkeys[5]));
    _mm_storeu_si128((__m128i_u *)subkeys[16], _mm_aesimc_si128(*(__m128i*)subkeys[4]));
    _mm_storeu_si128((__m128i_u *)subkeys[17], _mm_aesimc_si128(*(__m128i*)subkeys[3]));
    _mm_storeu_si128((__m128i_u *)subkeys[18], _mm_aesimc_si128(*(__m128i*)subkeys[2]));
    _mm_storeu_si128((__m128i_u *)subkeys[19], _mm_aesimc_si128(*(__m128i*)subkeys[1]));
}

int main() {
    uint8_t dec1[16];
    dec_init();
    aes_dec(computed);
    memcpy(dec1, plain, 16);

    memset(plain, 0, 16);
    aes_enc(plain);
    uint8_t m[16];
    memcpy(m, computed, 16);
    for(int i=0;i<16;i++) {
        m[i] ^= dec1[i];
        printf("%c", m[i]);
    }
    puts("");

    return 0;
}