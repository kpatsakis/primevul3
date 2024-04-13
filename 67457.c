static int ssl_parse_signature_algorithm( mbedtls_ssl_context *ssl,
                                          unsigned char **p,
                                          unsigned char *end,
                                          mbedtls_md_type_t *md_alg,
                                          mbedtls_pk_type_t *pk_alg )
{
    ((void) ssl);
    *md_alg = MBEDTLS_MD_NONE;
    *pk_alg = MBEDTLS_PK_NONE;

    /* Only in TLS 1.2 */
    if( ssl->minor_ver != MBEDTLS_SSL_MINOR_VERSION_3 )
    {
        return( 0 );
    }

    if( (*p) + 2 > end )
        return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_KEY_EXCHANGE );

    /*
     * Get hash algorithm
     */
    if( ( *md_alg = mbedtls_ssl_md_alg_from_hash( (*p)[0] ) ) == MBEDTLS_MD_NONE )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "Server used unsupported "
                            "HashAlgorithm %d", *(p)[0] ) );
        return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_KEY_EXCHANGE );
    }

    /*
     * Get signature algorithm
     */
    if( ( *pk_alg = mbedtls_ssl_pk_alg_from_sig( (*p)[1] ) ) == MBEDTLS_PK_NONE )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "server used unsupported "
                            "SignatureAlgorithm %d", (*p)[1] ) );
        return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_KEY_EXCHANGE );
    }

    /*
     * Check if the hash is acceptable
     */
    if( mbedtls_ssl_check_sig_hash( ssl, *md_alg ) != 0 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "server used HashAlgorithm %d that was not offered",
                                    *(p)[0] ) );
        return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_KEY_EXCHANGE );
    }

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "Server used SignatureAlgorithm %d", (*p)[1] ) );
    MBEDTLS_SSL_DEBUG_MSG( 2, ( "Server used HashAlgorithm %d", (*p)[0] ) );
    *p += 2;

    return( 0 );
}
