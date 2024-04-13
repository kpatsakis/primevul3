static void ssl_write_signature_algorithms_ext( mbedtls_ssl_context *ssl,
                                                unsigned char *buf,
                                                size_t *olen )
{
    unsigned char *p = buf;
    const unsigned char *end = ssl->out_msg + MBEDTLS_SSL_MAX_CONTENT_LEN;
    size_t sig_alg_len = 0;
    const int *md;
#if defined(MBEDTLS_RSA_C) || defined(MBEDTLS_ECDSA_C)
    unsigned char *sig_alg_list = buf + 6;
#endif

    *olen = 0;

    if( ssl->conf->max_minor_ver != MBEDTLS_SSL_MINOR_VERSION_3 )
        return;

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, adding signature_algorithms extension" ) );

    for( md = ssl->conf->sig_hashes; *md != MBEDTLS_MD_NONE; md++ )
    {
#if defined(MBEDTLS_ECDSA_C)
        sig_alg_len += 2;
#endif
#if defined(MBEDTLS_RSA_C)
        sig_alg_len += 2;
#endif
    }

    if( end < p || (size_t)( end - p ) < sig_alg_len + 6 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "buffer too small" ) );
        return;
    }

    /*
     * Prepare signature_algorithms extension (TLS 1.2)
     */
    sig_alg_len = 0;

    for( md = ssl->conf->sig_hashes; *md != MBEDTLS_MD_NONE; md++ )
    {
#if defined(MBEDTLS_ECDSA_C)
        sig_alg_list[sig_alg_len++] = mbedtls_ssl_hash_from_md_alg( *md );
        sig_alg_list[sig_alg_len++] = MBEDTLS_SSL_SIG_ECDSA;
#endif
#if defined(MBEDTLS_RSA_C)
        sig_alg_list[sig_alg_len++] = mbedtls_ssl_hash_from_md_alg( *md );
        sig_alg_list[sig_alg_len++] = MBEDTLS_SSL_SIG_RSA;
#endif
    }

    /*
     * enum {
     *     none(0), md5(1), sha1(2), sha224(3), sha256(4), sha384(5),
     *     sha512(6), (255)
     * } HashAlgorithm;
     *
     * enum { anonymous(0), rsa(1), dsa(2), ecdsa(3), (255) }
     *   SignatureAlgorithm;
     *
     * struct {
     *     HashAlgorithm hash;
     *     SignatureAlgorithm signature;
     * } SignatureAndHashAlgorithm;
     *
     * SignatureAndHashAlgorithm
     *   supported_signature_algorithms<2..2^16-2>;
     */
    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_SIG_ALG >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_SIG_ALG      ) & 0xFF );

    *p++ = (unsigned char)( ( ( sig_alg_len + 2 ) >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( ( sig_alg_len + 2 )      ) & 0xFF );

    *p++ = (unsigned char)( ( sig_alg_len >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( sig_alg_len      ) & 0xFF );

    *olen = 6 + sig_alg_len;
}
