static int ssl_write_client_hello( mbedtls_ssl_context *ssl )
{
    int ret;
    size_t i, n, olen, ext_len = 0;
    unsigned char *buf;
    unsigned char *p, *q;
    unsigned char offer_compress;
    const int *ciphersuites;
    const mbedtls_ssl_ciphersuite_t *ciphersuite_info;

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "=> write client hello" ) );

    if( ssl->conf->f_rng == NULL )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "no RNG provided") );
        return( MBEDTLS_ERR_SSL_NO_RNG );
    }

#if defined(MBEDTLS_SSL_RENEGOTIATION)
    if( ssl->renego_status == MBEDTLS_SSL_INITIAL_HANDSHAKE )
#endif
    {
        ssl->major_ver = ssl->conf->min_major_ver;
        ssl->minor_ver = ssl->conf->min_minor_ver;
    }

    if( ssl->conf->max_major_ver == 0 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "configured max major version is invalid, "
                            "consider using mbedtls_ssl_config_defaults()" ) );
        return( MBEDTLS_ERR_SSL_BAD_INPUT_DATA );
    }

    /*
     *     0  .   0   handshake type
     *     1  .   3   handshake length
     *     4  .   5   highest version supported
     *     6  .   9   current UNIX time
     *    10  .  37   random bytes
     */
    buf = ssl->out_msg;
    p = buf + 4;

    mbedtls_ssl_write_version( ssl->conf->max_major_ver, ssl->conf->max_minor_ver,
                       ssl->conf->transport, p );
    p += 2;

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, max version: [%d:%d]",
                   buf[4], buf[5] ) );

    if( ( ret = ssl_generate_random( ssl ) ) != 0 )
    {
        MBEDTLS_SSL_DEBUG_RET( 1, "ssl_generate_random", ret );
        return( ret );
    }

    memcpy( p, ssl->handshake->randbytes, 32 );
    MBEDTLS_SSL_DEBUG_BUF( 3, "client hello, random bytes", p, 32 );
    p += 32;

    /*
     *    38  .  38   session id length
     *    39  . 39+n  session id
     *   39+n . 39+n  DTLS only: cookie length (1 byte)
     *   40+n .  ..   DTSL only: cookie
     *   ..   . ..    ciphersuitelist length (2 bytes)
     *   ..   . ..    ciphersuitelist
     *   ..   . ..    compression methods length (1 byte)
     *   ..   . ..    compression methods
     *   ..   . ..    extensions length (2 bytes)
     *   ..   . ..    extensions
     */
    n = ssl->session_negotiate->id_len;

    if( n < 16 || n > 32 ||
#if defined(MBEDTLS_SSL_RENEGOTIATION)
        ssl->renego_status != MBEDTLS_SSL_INITIAL_HANDSHAKE ||
#endif
        ssl->handshake->resume == 0 )
    {
        n = 0;
    }

#if defined(MBEDTLS_SSL_SESSION_TICKETS)
    /*
     * RFC 5077 section 3.4: "When presenting a ticket, the client MAY
     * generate and include a Session ID in the TLS ClientHello."
     */
#if defined(MBEDTLS_SSL_RENEGOTIATION)
    if( ssl->renego_status == MBEDTLS_SSL_INITIAL_HANDSHAKE )
#endif
    {
        if( ssl->session_negotiate->ticket != NULL &&
                ssl->session_negotiate->ticket_len != 0 )
        {
            ret = ssl->conf->f_rng( ssl->conf->p_rng, ssl->session_negotiate->id, 32 );

            if( ret != 0 )
                return( ret );

            ssl->session_negotiate->id_len = n = 32;
        }
    }
#endif /* MBEDTLS_SSL_SESSION_TICKETS */

    *p++ = (unsigned char) n;

    for( i = 0; i < n; i++ )
        *p++ = ssl->session_negotiate->id[i];

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, session id len.: %d", n ) );
    MBEDTLS_SSL_DEBUG_BUF( 3,   "client hello, session id", buf + 39, n );

    /*
     * DTLS cookie
     */
#if defined(MBEDTLS_SSL_PROTO_DTLS)
    if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM )
    {
        if( ssl->handshake->verify_cookie == NULL )
        {
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "no verify cookie to send" ) );
            *p++ = 0;
        }
        else
        {
            MBEDTLS_SSL_DEBUG_BUF( 3, "client hello, cookie",
                              ssl->handshake->verify_cookie,
                              ssl->handshake->verify_cookie_len );

            *p++ = ssl->handshake->verify_cookie_len;
            memcpy( p, ssl->handshake->verify_cookie,
                       ssl->handshake->verify_cookie_len );
            p += ssl->handshake->verify_cookie_len;
        }
    }
#endif

    /*
     * Ciphersuite list
     */
    ciphersuites = ssl->conf->ciphersuite_list[ssl->minor_ver];

    /* Skip writing ciphersuite length for now */
    n = 0;
    q = p;
    p += 2;

    for( i = 0; ciphersuites[i] != 0; i++ )
    {
        ciphersuite_info = mbedtls_ssl_ciphersuite_from_id( ciphersuites[i] );

        if( ciphersuite_info == NULL )
            continue;

        if( ciphersuite_info->min_minor_ver > ssl->conf->max_minor_ver ||
            ciphersuite_info->max_minor_ver < ssl->conf->min_minor_ver )
            continue;

#if defined(MBEDTLS_SSL_PROTO_DTLS)
        if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM &&
            ( ciphersuite_info->flags & MBEDTLS_CIPHERSUITE_NODTLS ) )
            continue;
#endif

#if defined(MBEDTLS_ARC4_C)
        if( ssl->conf->arc4_disabled == MBEDTLS_SSL_ARC4_DISABLED &&
            ciphersuite_info->cipher == MBEDTLS_CIPHER_ARC4_128 )
            continue;
#endif

#if defined(MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED)
        if( ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_ECJPAKE &&
            mbedtls_ecjpake_check( &ssl->handshake->ecjpake_ctx ) != 0 )
            continue;
#endif

        MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, add ciphersuite: %04x",
                                    ciphersuites[i] ) );

        n++;
        *p++ = (unsigned char)( ciphersuites[i] >> 8 );
        *p++ = (unsigned char)( ciphersuites[i]      );
    }

    /*
     * Add TLS_EMPTY_RENEGOTIATION_INFO_SCSV
     */
#if defined(MBEDTLS_SSL_RENEGOTIATION)
    if( ssl->renego_status == MBEDTLS_SSL_INITIAL_HANDSHAKE )
#endif
    {
        *p++ = (unsigned char)( MBEDTLS_SSL_EMPTY_RENEGOTIATION_INFO >> 8 );
        *p++ = (unsigned char)( MBEDTLS_SSL_EMPTY_RENEGOTIATION_INFO      );
        n++;
    }

    /* Some versions of OpenSSL don't handle it correctly if not at end */
#if defined(MBEDTLS_SSL_FALLBACK_SCSV)
    if( ssl->conf->fallback == MBEDTLS_SSL_IS_FALLBACK )
    {
        MBEDTLS_SSL_DEBUG_MSG( 3, ( "adding FALLBACK_SCSV" ) );
        *p++ = (unsigned char)( MBEDTLS_SSL_FALLBACK_SCSV_VALUE >> 8 );
        *p++ = (unsigned char)( MBEDTLS_SSL_FALLBACK_SCSV_VALUE      );
        n++;
    }
#endif

    *q++ = (unsigned char)( n >> 7 );
    *q++ = (unsigned char)( n << 1 );

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, got %d ciphersuites", n ) );

#if defined(MBEDTLS_ZLIB_SUPPORT)
    offer_compress = 1;
#else
    offer_compress = 0;
#endif

    /*
     * We don't support compression with DTLS right now: is many records come
     * in the same datagram, uncompressing one could overwrite the next one.
     * We don't want to add complexity for handling that case unless there is
     * an actual need for it.
     */
#if defined(MBEDTLS_SSL_PROTO_DTLS)
    if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM )
        offer_compress = 0;
#endif

    if( offer_compress )
    {
        MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, compress len.: %d", 2 ) );
        MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, compress alg.: %d %d",
                            MBEDTLS_SSL_COMPRESS_DEFLATE, MBEDTLS_SSL_COMPRESS_NULL ) );

        *p++ = 2;
        *p++ = MBEDTLS_SSL_COMPRESS_DEFLATE;
        *p++ = MBEDTLS_SSL_COMPRESS_NULL;
    }
    else
    {
        MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, compress len.: %d", 1 ) );
        MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, compress alg.: %d",
                            MBEDTLS_SSL_COMPRESS_NULL ) );

        *p++ = 1;
        *p++ = MBEDTLS_SSL_COMPRESS_NULL;
    }

#if defined(MBEDTLS_SSL_SERVER_NAME_INDICATION)
    ssl_write_hostname_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

    /* Note that TLS_EMPTY_RENEGOTIATION_INFO_SCSV is always added
     * even if MBEDTLS_SSL_RENEGOTIATION is not defined. */
#if defined(MBEDTLS_SSL_RENEGOTIATION)
    ssl_write_renegotiation_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

#if defined(MBEDTLS_SSL_PROTO_TLS1_2) && \
    defined(MBEDTLS_KEY_EXCHANGE__WITH_CERT__ENABLED)
    ssl_write_signature_algorithms_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

#if defined(MBEDTLS_ECDH_C) || defined(MBEDTLS_ECDSA_C) || \
    defined(MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED)
    ssl_write_supported_elliptic_curves_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;

    ssl_write_supported_point_formats_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

#if defined(MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED)
    ssl_write_ecjpake_kkpp_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

#if defined(MBEDTLS_SSL_MAX_FRAGMENT_LENGTH)
    ssl_write_max_fragment_length_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

#if defined(MBEDTLS_SSL_TRUNCATED_HMAC)
    ssl_write_truncated_hmac_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

#if defined(MBEDTLS_SSL_ENCRYPT_THEN_MAC)
    ssl_write_encrypt_then_mac_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

#if defined(MBEDTLS_SSL_EXTENDED_MASTER_SECRET)
    ssl_write_extended_ms_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

#if defined(MBEDTLS_SSL_ALPN)
    ssl_write_alpn_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

#if defined(MBEDTLS_SSL_SESSION_TICKETS)
    ssl_write_session_ticket_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

    /* olen unused if all extensions are disabled */
    ((void) olen);

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, total extension length: %d",
                   ext_len ) );

    if( ext_len > 0 )
    {
        *p++ = (unsigned char)( ( ext_len >> 8 ) & 0xFF );
        *p++ = (unsigned char)( ( ext_len      ) & 0xFF );
        p += ext_len;
    }

    ssl->out_msglen  = p - buf;
    ssl->out_msgtype = MBEDTLS_SSL_MSG_HANDSHAKE;
    ssl->out_msg[0]  = MBEDTLS_SSL_HS_CLIENT_HELLO;

    ssl->state++;

#if defined(MBEDTLS_SSL_PROTO_DTLS)
    if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM )
        mbedtls_ssl_send_flight_completed( ssl );
#endif

    if( ( ret = mbedtls_ssl_write_record( ssl ) ) != 0 )
    {
        MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ssl_write_record", ret );
        return( ret );
    }

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= write client hello" ) );

    return( 0 );
}
