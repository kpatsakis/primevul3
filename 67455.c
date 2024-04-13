static int ssl_parse_server_hello( mbedtls_ssl_context *ssl )
{
    int ret, i;
    size_t n;
    size_t ext_len;
    unsigned char *buf, *ext;
    unsigned char comp;
#if defined(MBEDTLS_ZLIB_SUPPORT)
    int accept_comp;
#endif
#if defined(MBEDTLS_SSL_RENEGOTIATION)
    int renegotiation_info_seen = 0;
#endif
    int handshake_failure = 0;
    const mbedtls_ssl_ciphersuite_t *suite_info;

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "=> parse server hello" ) );

    buf = ssl->in_msg;

    if( ( ret = mbedtls_ssl_read_record( ssl ) ) != 0 )
    {
        /* No alert on a read error. */
        MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ssl_read_record", ret );
        return( ret );
    }

    if( ssl->in_msgtype != MBEDTLS_SSL_MSG_HANDSHAKE )
    {
#if defined(MBEDTLS_SSL_RENEGOTIATION)
        if( ssl->renego_status == MBEDTLS_SSL_RENEGOTIATION_IN_PROGRESS )
        {
            ssl->renego_records_seen++;

            if( ssl->conf->renego_max_records >= 0 &&
                ssl->renego_records_seen > ssl->conf->renego_max_records )
            {
                MBEDTLS_SSL_DEBUG_MSG( 1, ( "renegotiation requested, "
                                    "but not honored by server" ) );
                return( MBEDTLS_ERR_SSL_UNEXPECTED_MESSAGE );
            }

            MBEDTLS_SSL_DEBUG_MSG( 1, ( "non-handshake message during renego" ) );

            ssl->keep_current_message = 1;
            return( MBEDTLS_ERR_SSL_WAITING_SERVER_HELLO_RENEGO );
        }
#endif /* MBEDTLS_SSL_RENEGOTIATION */

        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad server hello message" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_UNEXPECTED_MESSAGE );
        return( MBEDTLS_ERR_SSL_UNEXPECTED_MESSAGE );
    }

#if defined(MBEDTLS_SSL_PROTO_DTLS)
    if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM )
    {
        if( buf[0] == MBEDTLS_SSL_HS_HELLO_VERIFY_REQUEST )
        {
            MBEDTLS_SSL_DEBUG_MSG( 2, ( "received hello verify request" ) );
            MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= parse server hello" ) );
            return( ssl_parse_hello_verify_request( ssl ) );
        }
        else
        {
            /* We made it through the verification process */
            mbedtls_free( ssl->handshake->verify_cookie );
            ssl->handshake->verify_cookie = NULL;
            ssl->handshake->verify_cookie_len = 0;
        }
    }
#endif /* MBEDTLS_SSL_PROTO_DTLS */

    if( ssl->in_hslen < 38 + mbedtls_ssl_hs_hdr_len( ssl ) ||
        buf[0] != MBEDTLS_SSL_HS_SERVER_HELLO )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad server hello message" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
        return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_HELLO );
    }

    /*
     *  0   .  1    server_version
     *  2   . 33    random (maybe including 4 bytes of Unix time)
     * 34   . 34    session_id length = n
     * 35   . 34+n  session_id
     * 35+n . 36+n  cipher_suite
     * 37+n . 37+n  compression_method
     *
     * 38+n . 39+n  extensions length (optional)
     * 40+n .  ..   extensions
     */
    buf += mbedtls_ssl_hs_hdr_len( ssl );

    MBEDTLS_SSL_DEBUG_BUF( 3, "server hello, version", buf + 0, 2 );
    mbedtls_ssl_read_version( &ssl->major_ver, &ssl->minor_ver,
                      ssl->conf->transport, buf + 0 );

    if( ssl->major_ver < ssl->conf->min_major_ver ||
        ssl->minor_ver < ssl->conf->min_minor_ver ||
        ssl->major_ver > ssl->conf->max_major_ver ||
        ssl->minor_ver > ssl->conf->max_minor_ver )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "server version out of bounds - "
                            " min: [%d:%d], server: [%d:%d], max: [%d:%d]",
                            ssl->conf->min_major_ver, ssl->conf->min_minor_ver,
                            ssl->major_ver, ssl->minor_ver,
                            ssl->conf->max_major_ver, ssl->conf->max_minor_ver ) );

        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                     MBEDTLS_SSL_ALERT_MSG_PROTOCOL_VERSION );

        return( MBEDTLS_ERR_SSL_BAD_HS_PROTOCOL_VERSION );
    }

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "server hello, current time: %lu",
                           ( (uint32_t) buf[2] << 24 ) |
                           ( (uint32_t) buf[3] << 16 ) |
                           ( (uint32_t) buf[4] <<  8 ) |
                           ( (uint32_t) buf[5]       ) ) );

    memcpy( ssl->handshake->randbytes + 32, buf + 2, 32 );

    n = buf[34];

    MBEDTLS_SSL_DEBUG_BUF( 3,   "server hello, random bytes", buf + 2, 32 );

    if( n > 32 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad server hello message" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
        return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_HELLO );
    }

    if( ssl->in_hslen > mbedtls_ssl_hs_hdr_len( ssl ) + 39 + n )
    {
        ext_len = ( ( buf[38 + n] <<  8 )
                  | ( buf[39 + n]       ) );

        if( ( ext_len > 0 && ext_len < 4 ) ||
            ssl->in_hslen != mbedtls_ssl_hs_hdr_len( ssl ) + 40 + n + ext_len )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad server hello message" ) );
            mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                            MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
            return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_HELLO );
        }
    }
    else if( ssl->in_hslen == mbedtls_ssl_hs_hdr_len( ssl ) + 38 + n )
    {
        ext_len = 0;
    }
    else
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad server hello message" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
        return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_HELLO );
    }

    /* ciphersuite (used later) */
    i = ( buf[35 + n] << 8 ) | buf[36 + n];

    /*
     * Read and check compression
     */
    comp = buf[37 + n];

#if defined(MBEDTLS_ZLIB_SUPPORT)
    /* See comments in ssl_write_client_hello() */
#if defined(MBEDTLS_SSL_PROTO_DTLS)
    if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM )
        accept_comp = 0;
    else
#endif
        accept_comp = 1;

    if( comp != MBEDTLS_SSL_COMPRESS_NULL &&
        ( comp != MBEDTLS_SSL_COMPRESS_DEFLATE || accept_comp == 0 ) )
#else /* MBEDTLS_ZLIB_SUPPORT */
    if( comp != MBEDTLS_SSL_COMPRESS_NULL )
#endif/* MBEDTLS_ZLIB_SUPPORT */
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "server hello, bad compression: %d", comp ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_ILLEGAL_PARAMETER );
        return( MBEDTLS_ERR_SSL_FEATURE_UNAVAILABLE );
    }

    /*
     * Initialize update checksum functions
     */
    ssl->transform_negotiate->ciphersuite_info = mbedtls_ssl_ciphersuite_from_id( i );

    if( ssl->transform_negotiate->ciphersuite_info == NULL )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "ciphersuite info for %04x not found", i ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_INTERNAL_ERROR );
        return( MBEDTLS_ERR_SSL_BAD_INPUT_DATA );
    }

    mbedtls_ssl_optimize_checksum( ssl, ssl->transform_negotiate->ciphersuite_info );

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "server hello, session id len.: %d", n ) );
    MBEDTLS_SSL_DEBUG_BUF( 3,   "server hello, session id", buf + 35, n );

    /*
     * Check if the session can be resumed
     */
    if( ssl->handshake->resume == 0 || n == 0 ||
#if defined(MBEDTLS_SSL_RENEGOTIATION)
        ssl->renego_status != MBEDTLS_SSL_INITIAL_HANDSHAKE ||
#endif
        ssl->session_negotiate->ciphersuite != i ||
        ssl->session_negotiate->compression != comp ||
        ssl->session_negotiate->id_len != n ||
        memcmp( ssl->session_negotiate->id, buf + 35, n ) != 0 )
    {
        ssl->state++;
        ssl->handshake->resume = 0;
#if defined(MBEDTLS_HAVE_TIME)
        ssl->session_negotiate->start = mbedtls_time( NULL );
#endif
        ssl->session_negotiate->ciphersuite = i;
        ssl->session_negotiate->compression = comp;
        ssl->session_negotiate->id_len = n;
        memcpy( ssl->session_negotiate->id, buf + 35, n );
    }
    else
    {
        ssl->state = MBEDTLS_SSL_SERVER_CHANGE_CIPHER_SPEC;

        if( ( ret = mbedtls_ssl_derive_keys( ssl ) ) != 0 )
        {
            MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ssl_derive_keys", ret );
            mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                            MBEDTLS_SSL_ALERT_MSG_INTERNAL_ERROR );
            return( ret );
        }
    }

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "%s session has been resumed",
                   ssl->handshake->resume ? "a" : "no" ) );

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "server hello, chosen ciphersuite: %04x", i ) );
    MBEDTLS_SSL_DEBUG_MSG( 3, ( "server hello, compress alg.: %d", buf[37 + n] ) );

    suite_info = mbedtls_ssl_ciphersuite_from_id( ssl->session_negotiate->ciphersuite );
    if( suite_info == NULL
#if defined(MBEDTLS_ARC4_C)
            || ( ssl->conf->arc4_disabled &&
                suite_info->cipher == MBEDTLS_CIPHER_ARC4_128 )
#endif
        )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad server hello message" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_ILLEGAL_PARAMETER );
        return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_HELLO );
    }

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "server hello, chosen ciphersuite: %s", suite_info->name ) );

    i = 0;
    while( 1 )
    {
        if( ssl->conf->ciphersuite_list[ssl->minor_ver][i] == 0 )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad server hello message" ) );
            mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                            MBEDTLS_SSL_ALERT_MSG_ILLEGAL_PARAMETER );
            return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_HELLO );
        }

        if( ssl->conf->ciphersuite_list[ssl->minor_ver][i++] ==
            ssl->session_negotiate->ciphersuite )
        {
            break;
        }
    }

    if( comp != MBEDTLS_SSL_COMPRESS_NULL
#if defined(MBEDTLS_ZLIB_SUPPORT)
        && comp != MBEDTLS_SSL_COMPRESS_DEFLATE
#endif
      )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad server hello message" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_ILLEGAL_PARAMETER );
        return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_HELLO );
    }
    ssl->session_negotiate->compression = comp;

    ext = buf + 40 + n;

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "server hello, total extension length: %d", ext_len ) );

    while( ext_len )
    {
        unsigned int ext_id   = ( ( ext[0] <<  8 )
                                | ( ext[1]       ) );
        unsigned int ext_size = ( ( ext[2] <<  8 )
                                | ( ext[3]       ) );

        if( ext_size + 4 > ext_len )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad server hello message" ) );
            mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                            MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
            return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_HELLO );
        }

        switch( ext_id )
        {
        case MBEDTLS_TLS_EXT_RENEGOTIATION_INFO:
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "found renegotiation extension" ) );
#if defined(MBEDTLS_SSL_RENEGOTIATION)
            renegotiation_info_seen = 1;
#endif

            if( ( ret = ssl_parse_renegotiation_info( ssl, ext + 4,
                                                      ext_size ) ) != 0 )
                return( ret );

            break;

#if defined(MBEDTLS_SSL_MAX_FRAGMENT_LENGTH)
        case MBEDTLS_TLS_EXT_MAX_FRAGMENT_LENGTH:
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "found max_fragment_length extension" ) );

            if( ( ret = ssl_parse_max_fragment_length_ext( ssl,
                            ext + 4, ext_size ) ) != 0 )
            {
                return( ret );
            }

            break;
#endif /* MBEDTLS_SSL_MAX_FRAGMENT_LENGTH */

#if defined(MBEDTLS_SSL_TRUNCATED_HMAC)
        case MBEDTLS_TLS_EXT_TRUNCATED_HMAC:
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "found truncated_hmac extension" ) );

            if( ( ret = ssl_parse_truncated_hmac_ext( ssl,
                            ext + 4, ext_size ) ) != 0 )
            {
                return( ret );
            }

            break;
#endif /* MBEDTLS_SSL_TRUNCATED_HMAC */

#if defined(MBEDTLS_SSL_ENCRYPT_THEN_MAC)
        case MBEDTLS_TLS_EXT_ENCRYPT_THEN_MAC:
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "found encrypt_then_mac extension" ) );

            if( ( ret = ssl_parse_encrypt_then_mac_ext( ssl,
                            ext + 4, ext_size ) ) != 0 )
            {
                return( ret );
            }

            break;
#endif /* MBEDTLS_SSL_ENCRYPT_THEN_MAC */

#if defined(MBEDTLS_SSL_EXTENDED_MASTER_SECRET)
        case MBEDTLS_TLS_EXT_EXTENDED_MASTER_SECRET:
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "found extended_master_secret extension" ) );

            if( ( ret = ssl_parse_extended_ms_ext( ssl,
                            ext + 4, ext_size ) ) != 0 )
            {
                return( ret );
            }

            break;
#endif /* MBEDTLS_SSL_EXTENDED_MASTER_SECRET */

#if defined(MBEDTLS_SSL_SESSION_TICKETS)
        case MBEDTLS_TLS_EXT_SESSION_TICKET:
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "found session_ticket extension" ) );

            if( ( ret = ssl_parse_session_ticket_ext( ssl,
                            ext + 4, ext_size ) ) != 0 )
            {
                return( ret );
            }

            break;
#endif /* MBEDTLS_SSL_SESSION_TICKETS */

#if defined(MBEDTLS_ECDH_C) || defined(MBEDTLS_ECDSA_C) || \
    defined(MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED)
        case MBEDTLS_TLS_EXT_SUPPORTED_POINT_FORMATS:
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "found supported_point_formats extension" ) );

            if( ( ret = ssl_parse_supported_point_formats_ext( ssl,
                            ext + 4, ext_size ) ) != 0 )
            {
                return( ret );
            }

            break;
#endif /* MBEDTLS_ECDH_C || MBEDTLS_ECDSA_C ||
          MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED */

#if defined(MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED)
        case MBEDTLS_TLS_EXT_ECJPAKE_KKPP:
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "found ecjpake_kkpp extension" ) );

            if( ( ret = ssl_parse_ecjpake_kkpp( ssl,
                            ext + 4, ext_size ) ) != 0 )
            {
                return( ret );
            }

            break;
#endif /* MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED */

#if defined(MBEDTLS_SSL_ALPN)
        case MBEDTLS_TLS_EXT_ALPN:
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "found alpn extension" ) );

            if( ( ret = ssl_parse_alpn_ext( ssl, ext + 4, ext_size ) ) != 0 )
                return( ret );

            break;
#endif /* MBEDTLS_SSL_ALPN */

        default:
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "unknown extension found: %d (ignoring)",
                           ext_id ) );
        }

        ext_len -= 4 + ext_size;
        ext += 4 + ext_size;

        if( ext_len > 0 && ext_len < 4 )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad server hello message" ) );
            return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_HELLO );
        }
    }

    /*
     * Renegotiation security checks
     */
    if( ssl->secure_renegotiation == MBEDTLS_SSL_LEGACY_RENEGOTIATION &&
        ssl->conf->allow_legacy_renegotiation == MBEDTLS_SSL_LEGACY_BREAK_HANDSHAKE )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "legacy renegotiation, breaking off handshake" ) );
        handshake_failure = 1;
    }
#if defined(MBEDTLS_SSL_RENEGOTIATION)
    else if( ssl->renego_status == MBEDTLS_SSL_RENEGOTIATION_IN_PROGRESS &&
             ssl->secure_renegotiation == MBEDTLS_SSL_SECURE_RENEGOTIATION &&
             renegotiation_info_seen == 0 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "renegotiation_info extension missing (secure)" ) );
        handshake_failure = 1;
    }
    else if( ssl->renego_status == MBEDTLS_SSL_RENEGOTIATION_IN_PROGRESS &&
             ssl->secure_renegotiation == MBEDTLS_SSL_LEGACY_RENEGOTIATION &&
             ssl->conf->allow_legacy_renegotiation == MBEDTLS_SSL_LEGACY_NO_RENEGOTIATION )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "legacy renegotiation not allowed" ) );
        handshake_failure = 1;
    }
    else if( ssl->renego_status == MBEDTLS_SSL_RENEGOTIATION_IN_PROGRESS &&
             ssl->secure_renegotiation == MBEDTLS_SSL_LEGACY_RENEGOTIATION &&
             renegotiation_info_seen == 1 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "renegotiation_info extension present (legacy)" ) );
        handshake_failure = 1;
    }
#endif /* MBEDTLS_SSL_RENEGOTIATION */

    if( handshake_failure == 1 )
    {
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_HANDSHAKE_FAILURE );
        return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_HELLO );
    }

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= parse server hello" ) );

    return( 0 );
}
