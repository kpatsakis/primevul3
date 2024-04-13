static int ssl_write_server_hello( mbedtls_ssl_context *ssl )
{
#if defined(MBEDTLS_HAVE_TIME)
    mbedtls_time_t t;
#endif
    int ret;
    size_t olen, ext_len = 0, n;
    unsigned char *buf, *p;

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "=> write server hello" ) );

#if defined(MBEDTLS_SSL_DTLS_HELLO_VERIFY)
    if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM &&
        ssl->handshake->verify_cookie_len != 0 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 2, ( "client hello was not authenticated" ) );
        MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= write server hello" ) );

        return( ssl_write_hello_verify_request( ssl ) );
    }
#endif /* MBEDTLS_SSL_DTLS_HELLO_VERIFY */

    if( ssl->conf->f_rng == NULL )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "no RNG provided") );
        return( MBEDTLS_ERR_SSL_NO_RNG );
    }

    /*
     *     0  .   0   handshake type
     *     1  .   3   handshake length
     *     4  .   5   protocol version
     *     6  .   9   UNIX time()
     *    10  .  37   random bytes
     */
    buf = ssl->out_msg;
    p = buf + 4;

    mbedtls_ssl_write_version( ssl->major_ver, ssl->minor_ver,
                       ssl->conf->transport, p );
    p += 2;

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "server hello, chosen version: [%d:%d]",
                        buf[4], buf[5] ) );

#if defined(MBEDTLS_HAVE_TIME)
    t = mbedtls_time( NULL );
    *p++ = (unsigned char)( t >> 24 );
    *p++ = (unsigned char)( t >> 16 );
    *p++ = (unsigned char)( t >>  8 );
    *p++ = (unsigned char)( t       );

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "server hello, current time: %lu", t ) );
#else
    if( ( ret = ssl->conf->f_rng( ssl->conf->p_rng, p, 4 ) ) != 0 )
        return( ret );

    p += 4;
#endif /* MBEDTLS_HAVE_TIME */

    if( ( ret = ssl->conf->f_rng( ssl->conf->p_rng, p, 28 ) ) != 0 )
        return( ret );

    p += 28;

    memcpy( ssl->handshake->randbytes + 32, buf + 6, 32 );

    MBEDTLS_SSL_DEBUG_BUF( 3, "server hello, random bytes", buf + 6, 32 );

    /*
     * Resume is 0  by default, see ssl_handshake_init().
     * It may be already set to 1 by ssl_parse_session_ticket_ext().
     * If not, try looking up session ID in our cache.
     */
    if( ssl->handshake->resume == 0 &&
#if defined(MBEDTLS_SSL_RENEGOTIATION)
        ssl->renego_status == MBEDTLS_SSL_INITIAL_HANDSHAKE &&
#endif
        ssl->session_negotiate->id_len != 0 &&
        ssl->conf->f_get_cache != NULL &&
        ssl->conf->f_get_cache( ssl->conf->p_cache, ssl->session_negotiate ) == 0 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 3, ( "session successfully restored from cache" ) );
        ssl->handshake->resume = 1;
    }

    if( ssl->handshake->resume == 0 )
    {
        /*
         * New session, create a new session id,
         * unless we're about to issue a session ticket
         */
        ssl->state++;

#if defined(MBEDTLS_HAVE_TIME)
        ssl->session_negotiate->start = mbedtls_time( NULL );
#endif

#if defined(MBEDTLS_SSL_SESSION_TICKETS)
        if( ssl->handshake->new_session_ticket != 0 )
        {
            ssl->session_negotiate->id_len = n = 0;
            memset( ssl->session_negotiate->id, 0, 32 );
        }
        else
#endif /* MBEDTLS_SSL_SESSION_TICKETS */
        {
            ssl->session_negotiate->id_len = n = 32;
            if( ( ret = ssl->conf->f_rng( ssl->conf->p_rng, ssl->session_negotiate->id,
                                    n ) ) != 0 )
                return( ret );
        }
    }
    else
    {
        /*
         * Resuming a session
         */
        n = ssl->session_negotiate->id_len;
        ssl->state = MBEDTLS_SSL_SERVER_CHANGE_CIPHER_SPEC;

        if( ( ret = mbedtls_ssl_derive_keys( ssl ) ) != 0 )
        {
            MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ssl_derive_keys", ret );
            return( ret );
        }
    }

    /*
     *    38  .  38     session id length
     *    39  . 38+n    session id
     *   39+n . 40+n    chosen ciphersuite
     *   41+n . 41+n    chosen compression alg.
     *   42+n . 43+n    extensions length
     *   44+n . 43+n+m  extensions
     */
    *p++ = (unsigned char) ssl->session_negotiate->id_len;
    memcpy( p, ssl->session_negotiate->id, ssl->session_negotiate->id_len );
    p += ssl->session_negotiate->id_len;

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "server hello, session id len.: %d", n ) );
    MBEDTLS_SSL_DEBUG_BUF( 3,   "server hello, session id", buf + 39, n );
    MBEDTLS_SSL_DEBUG_MSG( 3, ( "%s session has been resumed",
                   ssl->handshake->resume ? "a" : "no" ) );

    *p++ = (unsigned char)( ssl->session_negotiate->ciphersuite >> 8 );
    *p++ = (unsigned char)( ssl->session_negotiate->ciphersuite      );
    *p++ = (unsigned char)( ssl->session_negotiate->compression      );

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "server hello, chosen ciphersuite: %s",
           mbedtls_ssl_get_ciphersuite_name( ssl->session_negotiate->ciphersuite ) ) );
    MBEDTLS_SSL_DEBUG_MSG( 3, ( "server hello, compress alg.: 0x%02X",
                   ssl->session_negotiate->compression ) );

    /* Do not write the extensions if the protocol is SSLv3 */
#if defined(MBEDTLS_SSL_PROTO_SSL3)
    if( ( ssl->major_ver != 3 ) || ( ssl->minor_ver != 0 ) )
    {
#endif

    /*
     *  First write extensions, then the total length
     */
    ssl_write_renegotiation_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;

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

#if defined(MBEDTLS_SSL_SESSION_TICKETS)
    ssl_write_session_ticket_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

#if defined(MBEDTLS_ECDH_C) || defined(MBEDTLS_ECDSA_C) || \
    defined(MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED)
    ssl_write_supported_point_formats_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

#if defined(MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED)
    ssl_write_ecjpake_kkpp_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

#if defined(MBEDTLS_SSL_ALPN)
    ssl_write_alpn_ext( ssl, p + 2 + ext_len, &olen );
    ext_len += olen;
#endif

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "server hello, total extension length: %d", ext_len ) );

    if( ext_len > 0 )
    {
        *p++ = (unsigned char)( ( ext_len >> 8 ) & 0xFF );
        *p++ = (unsigned char)( ( ext_len      ) & 0xFF );
        p += ext_len;
    }

#if defined(MBEDTLS_SSL_PROTO_SSL3)
    }
#endif

    ssl->out_msglen  = p - buf;
    ssl->out_msgtype = MBEDTLS_SSL_MSG_HANDSHAKE;
    ssl->out_msg[0]  = MBEDTLS_SSL_HS_SERVER_HELLO;

    ret = mbedtls_ssl_write_record( ssl );

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= write server hello" ) );

    return( ret );
}
