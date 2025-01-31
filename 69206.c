static int ssl_parse_encrypted_pms( mbedtls_ssl_context *ssl,
                                    const unsigned char *p,
                                    const unsigned char *end,
                                    size_t pms_offset )
{
    int ret;
    size_t len = mbedtls_pk_get_len( mbedtls_ssl_own_key( ssl ) );
    unsigned char *pms = ssl->handshake->premaster + pms_offset;
    unsigned char ver[2];
    unsigned char fake_pms[48], peer_pms[48];
    unsigned char mask;
    size_t i, peer_pmslen;
    unsigned int diff;

    if( ! mbedtls_pk_can_do( mbedtls_ssl_own_key( ssl ), MBEDTLS_PK_RSA ) )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "got no RSA private key" ) );
        return( MBEDTLS_ERR_SSL_PRIVATE_KEY_REQUIRED );
    }

    /*
     * Decrypt the premaster using own private RSA key
     */
#if defined(MBEDTLS_SSL_PROTO_TLS1) || defined(MBEDTLS_SSL_PROTO_TLS1_1) || \
    defined(MBEDTLS_SSL_PROTO_TLS1_2)
    if( ssl->minor_ver != MBEDTLS_SSL_MINOR_VERSION_0 )
    {
        if( *p++ != ( ( len >> 8 ) & 0xFF ) ||
            *p++ != ( ( len      ) & 0xFF ) )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client key exchange message" ) );
            return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_KEY_EXCHANGE );
        }
    }
#endif

    if( p + len != end )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client key exchange message" ) );
        return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_KEY_EXCHANGE );
    }

    mbedtls_ssl_write_version( ssl->handshake->max_major_ver,
                       ssl->handshake->max_minor_ver,
                       ssl->conf->transport, ver );

    /*
     * Protection against Bleichenbacher's attack: invalid PKCS#1 v1.5 padding
     * must not cause the connection to end immediately; instead, send a
     * bad_record_mac later in the handshake.
     * Also, avoid data-dependant branches here to protect against
     * timing-based variants.
     */
    ret = ssl->conf->f_rng( ssl->conf->p_rng, fake_pms, sizeof( fake_pms ) );
    if( ret != 0 )
        return( ret );

    ret = mbedtls_pk_decrypt( mbedtls_ssl_own_key( ssl ), p, len,
                      peer_pms, &peer_pmslen,
                      sizeof( peer_pms ),
                      ssl->conf->f_rng, ssl->conf->p_rng );

    diff  = (unsigned int) ret;
    diff |= peer_pmslen ^ 48;
    diff |= peer_pms[0] ^ ver[0];
    diff |= peer_pms[1] ^ ver[1];

#if defined(MBEDTLS_SSL_DEBUG_ALL)
    if( diff != 0 )
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client key exchange message" ) );
#endif

    if( sizeof( ssl->handshake->premaster ) < pms_offset ||
        sizeof( ssl->handshake->premaster ) - pms_offset < 48 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "should never happen" ) );
        return( MBEDTLS_ERR_SSL_INTERNAL_ERROR );
    }
    ssl->handshake->pmslen = 48;

    /* mask = diff ? 0xff : 0x00 using bit operations to avoid branches */
    /* MSVC has a warning about unary minus on unsigned, but this is
     * well-defined and precisely what we want to do here */
#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4146 )
#endif
    mask = - ( ( diff | - diff ) >> ( sizeof( unsigned int ) * 8 - 1 ) );
#if defined(_MSC_VER)
#pragma warning( pop )
#endif

    for( i = 0; i < ssl->handshake->pmslen; i++ )
        pms[i] = ( mask & fake_pms[i] ) | ( (~mask) & peer_pms[i] );

    return( 0 );
}
