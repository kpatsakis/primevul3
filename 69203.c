static int ssl_parse_client_hello( mbedtls_ssl_context *ssl )
{
    int ret, got_common_suite;
    size_t i, j;
    size_t ciph_offset, comp_offset, ext_offset;
    size_t msg_len, ciph_len, sess_len, comp_len, ext_len;
#if defined(MBEDTLS_SSL_PROTO_DTLS)
    size_t cookie_offset, cookie_len;
#endif
    unsigned char *buf, *p, *ext;
#if defined(MBEDTLS_SSL_RENEGOTIATION)
    int renegotiation_info_seen = 0;
#endif
    int handshake_failure = 0;
    const int *ciphersuites;
    const mbedtls_ssl_ciphersuite_t *ciphersuite_info;
    int major, minor;

    /* If there is no signature-algorithm extension present,
     * we need to fall back to the default values for allowed
     * signature-hash pairs. */
#if defined(MBEDTLS_SSL_PROTO_TLS1_2) && \
    defined(MBEDTLS_KEY_EXCHANGE__WITH_CERT__ENABLED)
    int sig_hash_alg_ext_present = 0;
#endif /* MBEDTLS_SSL_PROTO_TLS1_2 &&
          MBEDTLS_KEY_EXCHANGE__WITH_CERT__ENABLED */

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "=> parse client hello" ) );

#if defined(MBEDTLS_SSL_DTLS_ANTI_REPLAY)
read_record_header:
#endif
    /*
     * If renegotiating, then the input was read with mbedtls_ssl_read_record(),
     * otherwise read it ourselves manually in order to support SSLv2
     * ClientHello, which doesn't use the same record layer format.
     */
#if defined(MBEDTLS_SSL_RENEGOTIATION)
    if( ssl->renego_status == MBEDTLS_SSL_INITIAL_HANDSHAKE )
#endif
    {
        if( ( ret = mbedtls_ssl_fetch_input( ssl, 5 ) ) != 0 )
        {
            /* No alert on a read error. */
            MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ssl_fetch_input", ret );
            return( ret );
        }
    }

    buf = ssl->in_hdr;

#if defined(MBEDTLS_SSL_SRV_SUPPORT_SSLV2_CLIENT_HELLO)
#if defined(MBEDTLS_SSL_PROTO_DTLS)
    if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_STREAM )
#endif
        if( ( buf[0] & 0x80 ) != 0 )
            return( ssl_parse_client_hello_v2( ssl ) );
#endif

    MBEDTLS_SSL_DEBUG_BUF( 4, "record header", buf, mbedtls_ssl_hdr_len( ssl ) );

    /*
     * SSLv3/TLS Client Hello
     *
     * Record layer:
     *     0  .   0   message type
     *     1  .   2   protocol version
     *     3  .   11  DTLS: epoch + record sequence number
     *     3  .   4   message length
     */
    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello v3, message type: %d",
                   buf[0] ) );

    if( buf[0] != MBEDTLS_SSL_MSG_HANDSHAKE )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
        return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
    }

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello v3, message len.: %d",
                   ( ssl->in_len[0] << 8 ) | ssl->in_len[1] ) );

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello v3, protocol version: [%d:%d]",
                   buf[1], buf[2] ) );

    mbedtls_ssl_read_version( &major, &minor, ssl->conf->transport, buf + 1 );

    /* According to RFC 5246 Appendix E.1, the version here is typically
     * "{03,00}, the lowest version number supported by the client, [or] the
     * value of ClientHello.client_version", so the only meaningful check here
     * is the major version shouldn't be less than 3 */
    if( major < MBEDTLS_SSL_MAJOR_VERSION_3 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
        return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
    }

    /* For DTLS if this is the initial handshake, remember the client sequence
     * number to use it in our next message (RFC 6347 4.2.1) */
#if defined(MBEDTLS_SSL_PROTO_DTLS)
    if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM
#if defined(MBEDTLS_SSL_RENEGOTIATION)
        && ssl->renego_status == MBEDTLS_SSL_INITIAL_HANDSHAKE
#endif
        )
    {
        /* Epoch should be 0 for initial handshakes */
        if( ssl->in_ctr[0] != 0 || ssl->in_ctr[1] != 0 )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
            return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
        }

        memcpy( ssl->out_ctr + 2, ssl->in_ctr + 2, 6 );

#if defined(MBEDTLS_SSL_DTLS_ANTI_REPLAY)
        if( mbedtls_ssl_dtls_replay_check( ssl ) != 0 )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "replayed record, discarding" ) );
            ssl->next_record_offset = 0;
            ssl->in_left = 0;
            goto read_record_header;
        }

        /* No MAC to check yet, so we can update right now */
        mbedtls_ssl_dtls_replay_update( ssl );
#endif
    }
#endif /* MBEDTLS_SSL_PROTO_DTLS */

    msg_len = ( ssl->in_len[0] << 8 ) | ssl->in_len[1];

#if defined(MBEDTLS_SSL_RENEGOTIATION)
    if( ssl->renego_status != MBEDTLS_SSL_INITIAL_HANDSHAKE )
    {
        /* Set by mbedtls_ssl_read_record() */
        msg_len = ssl->in_hslen;
    }
    else
#endif
    {
        if( msg_len > MBEDTLS_SSL_MAX_CONTENT_LEN )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
            return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
        }

        if( ( ret = mbedtls_ssl_fetch_input( ssl,
                       mbedtls_ssl_hdr_len( ssl ) + msg_len ) ) != 0 )
        {
            MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ssl_fetch_input", ret );
            return( ret );
        }

    /* Done reading this record, get ready for the next one */
#if defined(MBEDTLS_SSL_PROTO_DTLS)
        if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM )
            ssl->next_record_offset = msg_len + mbedtls_ssl_hdr_len( ssl );
        else
#endif
            ssl->in_left = 0;
    }

    buf = ssl->in_msg;

    MBEDTLS_SSL_DEBUG_BUF( 4, "record contents", buf, msg_len );

    ssl->handshake->update_checksum( ssl, buf, msg_len );

    /*
     * Handshake layer:
     *     0  .   0   handshake type
     *     1  .   3   handshake length
     *     4  .   5   DTLS only: message seqence number
     *     6  .   8   DTLS only: fragment offset
     *     9  .  11   DTLS only: fragment length
     */
    if( msg_len < mbedtls_ssl_hs_hdr_len( ssl ) )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
        return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
    }

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello v3, handshake type: %d", buf[0] ) );

    if( buf[0] != MBEDTLS_SSL_HS_CLIENT_HELLO )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
        return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
    }

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello v3, handshake len.: %d",
                   ( buf[1] << 16 ) | ( buf[2] << 8 ) | buf[3] ) );

    /* We don't support fragmentation of ClientHello (yet?) */
    if( buf[1] != 0 ||
        msg_len != mbedtls_ssl_hs_hdr_len( ssl ) + ( ( buf[2] << 8 ) | buf[3] ) )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
        return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
    }

#if defined(MBEDTLS_SSL_PROTO_DTLS)
    if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM )
    {
        /*
         * Copy the client's handshake message_seq on initial handshakes,
         * check sequence number on renego.
         */
#if defined(MBEDTLS_SSL_RENEGOTIATION)
        if( ssl->renego_status == MBEDTLS_SSL_RENEGOTIATION_IN_PROGRESS )
        {
            /* This couldn't be done in ssl_prepare_handshake_record() */
            unsigned int cli_msg_seq = ( ssl->in_msg[4] << 8 ) |
                                         ssl->in_msg[5];

            if( cli_msg_seq != ssl->handshake->in_msg_seq )
            {
                MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message_seq: "
                                    "%d (expected %d)", cli_msg_seq,
                                    ssl->handshake->in_msg_seq ) );
                return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
            }

            ssl->handshake->in_msg_seq++;
        }
        else
#endif
        {
            unsigned int cli_msg_seq = ( ssl->in_msg[4] << 8 ) |
                                         ssl->in_msg[5];
            ssl->handshake->out_msg_seq = cli_msg_seq;
            ssl->handshake->in_msg_seq  = cli_msg_seq + 1;
        }

        /*
         * For now we don't support fragmentation, so make sure
         * fragment_offset == 0 and fragment_length == length
         */
        if( ssl->in_msg[6] != 0 || ssl->in_msg[7] != 0 || ssl->in_msg[8] != 0 ||
            memcmp( ssl->in_msg + 1, ssl->in_msg + 9, 3 ) != 0 )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "ClientHello fragmentation not supported" ) );
            return( MBEDTLS_ERR_SSL_FEATURE_UNAVAILABLE );
        }
    }
#endif /* MBEDTLS_SSL_PROTO_DTLS */

    buf += mbedtls_ssl_hs_hdr_len( ssl );
    msg_len -= mbedtls_ssl_hs_hdr_len( ssl );

    /*
     * ClientHello layer:
     *     0  .   1   protocol version
     *     2  .  33   random bytes (starting with 4 bytes of Unix time)
     *    34  .  35   session id length (1 byte)
     *    35  . 34+x  session id
     *   35+x . 35+x  DTLS only: cookie length (1 byte)
     *   36+x .  ..   DTLS only: cookie
     *    ..  .  ..   ciphersuite list length (2 bytes)
     *    ..  .  ..   ciphersuite list
     *    ..  .  ..   compression alg. list length (1 byte)
     *    ..  .  ..   compression alg. list
     *    ..  .  ..   extensions length (2 bytes, optional)
     *    ..  .  ..   extensions (optional)
     */

    /*
     * Minimal length (with everything empty and extensions ommitted) is
     * 2 + 32 + 1 + 2 + 1 = 38 bytes. Check that first, so that we can
     * read at least up to session id length without worrying.
     */
    if( msg_len < 38 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
        return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
    }

    /*
     * Check and save the protocol version
     */
    MBEDTLS_SSL_DEBUG_BUF( 3, "client hello, version", buf, 2 );

    mbedtls_ssl_read_version( &ssl->major_ver, &ssl->minor_ver,
                      ssl->conf->transport, buf );

    ssl->handshake->max_major_ver = ssl->major_ver;
    ssl->handshake->max_minor_ver = ssl->minor_ver;

    if( ssl->major_ver < ssl->conf->min_major_ver ||
        ssl->minor_ver < ssl->conf->min_minor_ver )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "client only supports ssl smaller than minimum"
                            " [%d:%d] < [%d:%d]",
                            ssl->major_ver, ssl->minor_ver,
                            ssl->conf->min_major_ver, ssl->conf->min_minor_ver ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                     MBEDTLS_SSL_ALERT_MSG_PROTOCOL_VERSION );
        return( MBEDTLS_ERR_SSL_BAD_HS_PROTOCOL_VERSION );
    }

    if( ssl->major_ver > ssl->conf->max_major_ver )
    {
        ssl->major_ver = ssl->conf->max_major_ver;
        ssl->minor_ver = ssl->conf->max_minor_ver;
    }
    else if( ssl->minor_ver > ssl->conf->max_minor_ver )
        ssl->minor_ver = ssl->conf->max_minor_ver;

    /*
     * Save client random (inc. Unix time)
     */
    MBEDTLS_SSL_DEBUG_BUF( 3, "client hello, random bytes", buf + 2, 32 );

    memcpy( ssl->handshake->randbytes, buf + 2, 32 );

    /*
     * Check the session ID length and save session ID
     */
    sess_len = buf[34];

    if( sess_len > sizeof( ssl->session_negotiate->id ) ||
        sess_len + 34 + 2 > msg_len ) /* 2 for cipherlist length field */
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
        return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
    }

    MBEDTLS_SSL_DEBUG_BUF( 3, "client hello, session id", buf + 35, sess_len );

    ssl->session_negotiate->id_len = sess_len;
    memset( ssl->session_negotiate->id, 0,
            sizeof( ssl->session_negotiate->id ) );
    memcpy( ssl->session_negotiate->id, buf + 35,
            ssl->session_negotiate->id_len );

    /*
     * Check the cookie length and content
     */
#if defined(MBEDTLS_SSL_PROTO_DTLS)
    if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM )
    {
        cookie_offset = 35 + sess_len;
        cookie_len = buf[cookie_offset];

        if( cookie_offset + 1 + cookie_len + 2 > msg_len )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
            mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                            MBEDTLS_SSL_ALERT_MSG_PROTOCOL_VERSION );
            return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
        }

        MBEDTLS_SSL_DEBUG_BUF( 3, "client hello, cookie",
                       buf + cookie_offset + 1, cookie_len );

#if defined(MBEDTLS_SSL_DTLS_HELLO_VERIFY)
        if( ssl->conf->f_cookie_check != NULL
#if defined(MBEDTLS_SSL_RENEGOTIATION)
            && ssl->renego_status == MBEDTLS_SSL_INITIAL_HANDSHAKE
#endif
            )
        {
            if( ssl->conf->f_cookie_check( ssl->conf->p_cookie,
                                     buf + cookie_offset + 1, cookie_len,
                                     ssl->cli_id, ssl->cli_id_len ) != 0 )
            {
                MBEDTLS_SSL_DEBUG_MSG( 2, ( "cookie verification failed" ) );
                ssl->handshake->verify_cookie_len = 1;
            }
            else
            {
                MBEDTLS_SSL_DEBUG_MSG( 2, ( "cookie verification passed" ) );
                ssl->handshake->verify_cookie_len = 0;
            }
        }
        else
#endif /* MBEDTLS_SSL_DTLS_HELLO_VERIFY */
        {
            /* We know we didn't send a cookie, so it should be empty */
            if( cookie_len != 0 )
            {
                /* This may be an attacker's probe, so don't send an alert */
                MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
                return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
            }

            MBEDTLS_SSL_DEBUG_MSG( 2, ( "cookie verification skipped" ) );
        }

    /*
     * Check the ciphersuitelist length (will be parsed later)
     */
        ciph_offset = cookie_offset + 1 + cookie_len;
    }
    else
#endif /* MBEDTLS_SSL_PROTO_DTLS */
        ciph_offset = 35 + sess_len;

    ciph_len = ( buf[ciph_offset + 0] << 8 )
             | ( buf[ciph_offset + 1]      );

    if( ciph_len < 2 ||
        ciph_len + 2 + ciph_offset + 1 > msg_len || /* 1 for comp. alg. len */
        ( ciph_len % 2 ) != 0 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
        return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
    }

    MBEDTLS_SSL_DEBUG_BUF( 3, "client hello, ciphersuitelist",
                   buf + ciph_offset + 2,  ciph_len );

    /*
     * Check the compression algorithms length and pick one
     */
    comp_offset = ciph_offset + 2 + ciph_len;

    comp_len = buf[comp_offset];

    if( comp_len < 1 ||
        comp_len > 16 ||
        comp_len + comp_offset + 1 > msg_len )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
        return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
    }

    MBEDTLS_SSL_DEBUG_BUF( 3, "client hello, compression",
                      buf + comp_offset + 1, comp_len );

    ssl->session_negotiate->compression = MBEDTLS_SSL_COMPRESS_NULL;
#if defined(MBEDTLS_ZLIB_SUPPORT)
    for( i = 0; i < comp_len; ++i )
    {
        if( buf[comp_offset + 1 + i] == MBEDTLS_SSL_COMPRESS_DEFLATE )
        {
            ssl->session_negotiate->compression = MBEDTLS_SSL_COMPRESS_DEFLATE;
            break;
        }
    }
#endif

    /* See comments in ssl_write_client_hello() */
#if defined(MBEDTLS_SSL_PROTO_DTLS)
    if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM )
        ssl->session_negotiate->compression = MBEDTLS_SSL_COMPRESS_NULL;
#endif

    /* Do not parse the extensions if the protocol is SSLv3 */
#if defined(MBEDTLS_SSL_PROTO_SSL3)
    if( ( ssl->major_ver != 3 ) || ( ssl->minor_ver != 0 ) )
    {
#endif
        /*
         * Check the extension length
         */
        ext_offset = comp_offset + 1 + comp_len;
        if( msg_len > ext_offset )
        {
            if( msg_len < ext_offset + 2 )
            {
                MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
                mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                                MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
                return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
            }

            ext_len = ( buf[ext_offset + 0] << 8 )
                    | ( buf[ext_offset + 1]      );

            if( ( ext_len > 0 && ext_len < 4 ) ||
                msg_len != ext_offset + 2 + ext_len )
            {
                MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
                mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                                MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
                return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
            }
        }
        else
            ext_len = 0;

        ext = buf + ext_offset + 2;
        MBEDTLS_SSL_DEBUG_BUF( 3, "client hello extensions", ext, ext_len );

        while( ext_len != 0 )
        {
            unsigned int ext_id   = ( ( ext[0] <<  8 )
                                    | ( ext[1]       ) );
            unsigned int ext_size = ( ( ext[2] <<  8 )
                                    | ( ext[3]       ) );

            if( ext_size + 4 > ext_len )
            {
                MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
                mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                                MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
                return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
            }
            switch( ext_id )
            {
#if defined(MBEDTLS_SSL_SERVER_NAME_INDICATION)
            case MBEDTLS_TLS_EXT_SERVERNAME:
                MBEDTLS_SSL_DEBUG_MSG( 3, ( "found ServerName extension" ) );
                if( ssl->conf->f_sni == NULL )
                    break;

                ret = ssl_parse_servername_ext( ssl, ext + 4, ext_size );
                if( ret != 0 )
                    return( ret );
                break;
#endif /* MBEDTLS_SSL_SERVER_NAME_INDICATION */

            case MBEDTLS_TLS_EXT_RENEGOTIATION_INFO:
                MBEDTLS_SSL_DEBUG_MSG( 3, ( "found renegotiation extension" ) );
#if defined(MBEDTLS_SSL_RENEGOTIATION)
                renegotiation_info_seen = 1;
#endif

                ret = ssl_parse_renegotiation_info( ssl, ext + 4, ext_size );
                if( ret != 0 )
                    return( ret );
                break;

#if defined(MBEDTLS_SSL_PROTO_TLS1_2) && \
    defined(MBEDTLS_KEY_EXCHANGE__WITH_CERT__ENABLED)
            case MBEDTLS_TLS_EXT_SIG_ALG:
                    MBEDTLS_SSL_DEBUG_MSG( 3, ( "found signature_algorithms extension" ) );
#if defined(MBEDTLS_SSL_RENEGOTIATION)
                if( ssl->renego_status == MBEDTLS_SSL_RENEGOTIATION_IN_PROGRESS )
                    break;
#endif
                ret = ssl_parse_signature_algorithms_ext( ssl, ext + 4, ext_size );
                if( ret != 0 )
                    return( ret );

                sig_hash_alg_ext_present = 1;
                break;
#endif /* MBEDTLS_SSL_PROTO_TLS1_2 &&
          MBEDTLS_KEY_EXCHANGE__WITH_CERT__ENABLED */

#if defined(MBEDTLS_ECDH_C) || defined(MBEDTLS_ECDSA_C) || \
    defined(MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED)
            case MBEDTLS_TLS_EXT_SUPPORTED_ELLIPTIC_CURVES:
                MBEDTLS_SSL_DEBUG_MSG( 3, ( "found supported elliptic curves extension" ) );

                ret = ssl_parse_supported_elliptic_curves( ssl, ext + 4, ext_size );
                if( ret != 0 )
                    return( ret );
                break;

            case MBEDTLS_TLS_EXT_SUPPORTED_POINT_FORMATS:
                MBEDTLS_SSL_DEBUG_MSG( 3, ( "found supported point formats extension" ) );
                ssl->handshake->cli_exts |= MBEDTLS_TLS_EXT_SUPPORTED_POINT_FORMATS_PRESENT;

                ret = ssl_parse_supported_point_formats( ssl, ext + 4, ext_size );
                if( ret != 0 )
                    return( ret );
                break;
#endif /* MBEDTLS_ECDH_C || MBEDTLS_ECDSA_C ||
          MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED */

#if defined(MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED)
            case MBEDTLS_TLS_EXT_ECJPAKE_KKPP:
                MBEDTLS_SSL_DEBUG_MSG( 3, ( "found ecjpake kkpp extension" ) );

                ret = ssl_parse_ecjpake_kkpp( ssl, ext + 4, ext_size );
                if( ret != 0 )
                    return( ret );
                break;
#endif /* MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED */

#if defined(MBEDTLS_SSL_MAX_FRAGMENT_LENGTH)
            case MBEDTLS_TLS_EXT_MAX_FRAGMENT_LENGTH:
                MBEDTLS_SSL_DEBUG_MSG( 3, ( "found max fragment length extension" ) );

                ret = ssl_parse_max_fragment_length_ext( ssl, ext + 4, ext_size );
                if( ret != 0 )
                    return( ret );
                break;
#endif /* MBEDTLS_SSL_MAX_FRAGMENT_LENGTH */

#if defined(MBEDTLS_SSL_TRUNCATED_HMAC)
            case MBEDTLS_TLS_EXT_TRUNCATED_HMAC:
                MBEDTLS_SSL_DEBUG_MSG( 3, ( "found truncated hmac extension" ) );

                ret = ssl_parse_truncated_hmac_ext( ssl, ext + 4, ext_size );
                if( ret != 0 )
                    return( ret );
                break;
#endif /* MBEDTLS_SSL_TRUNCATED_HMAC */

#if defined(MBEDTLS_SSL_ENCRYPT_THEN_MAC)
            case MBEDTLS_TLS_EXT_ENCRYPT_THEN_MAC:
                MBEDTLS_SSL_DEBUG_MSG( 3, ( "found encrypt then mac extension" ) );

                ret = ssl_parse_encrypt_then_mac_ext( ssl, ext + 4, ext_size );
                if( ret != 0 )
                    return( ret );
                break;
#endif /* MBEDTLS_SSL_ENCRYPT_THEN_MAC */

#if defined(MBEDTLS_SSL_EXTENDED_MASTER_SECRET)
            case MBEDTLS_TLS_EXT_EXTENDED_MASTER_SECRET:
                MBEDTLS_SSL_DEBUG_MSG( 3, ( "found extended master secret extension" ) );

                ret = ssl_parse_extended_ms_ext( ssl, ext + 4, ext_size );
                if( ret != 0 )
                    return( ret );
                break;
#endif /* MBEDTLS_SSL_EXTENDED_MASTER_SECRET */

#if defined(MBEDTLS_SSL_SESSION_TICKETS)
            case MBEDTLS_TLS_EXT_SESSION_TICKET:
                MBEDTLS_SSL_DEBUG_MSG( 3, ( "found session ticket extension" ) );

                ret = ssl_parse_session_ticket_ext( ssl, ext + 4, ext_size );
                if( ret != 0 )
                    return( ret );
                break;
#endif /* MBEDTLS_SSL_SESSION_TICKETS */

#if defined(MBEDTLS_SSL_ALPN)
            case MBEDTLS_TLS_EXT_ALPN:
                MBEDTLS_SSL_DEBUG_MSG( 3, ( "found alpn extension" ) );

                ret = ssl_parse_alpn_ext( ssl, ext + 4, ext_size );
                if( ret != 0 )
                    return( ret );
                break;
#endif /* MBEDTLS_SSL_SESSION_TICKETS */

            default:
                MBEDTLS_SSL_DEBUG_MSG( 3, ( "unknown extension found: %d (ignoring)",
                               ext_id ) );
            }

            ext_len -= 4 + ext_size;
            ext += 4 + ext_size;

            if( ext_len > 0 && ext_len < 4 )
            {
                MBEDTLS_SSL_DEBUG_MSG( 1, ( "bad client hello message" ) );
                mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                                MBEDTLS_SSL_ALERT_MSG_DECODE_ERROR );
                return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
            }
        }
#if defined(MBEDTLS_SSL_PROTO_SSL3)
    }
#endif

#if defined(MBEDTLS_SSL_FALLBACK_SCSV)
    for( i = 0, p = buf + ciph_offset + 2; i < ciph_len; i += 2, p += 2 )
    {
        if( p[0] == (unsigned char)( ( MBEDTLS_SSL_FALLBACK_SCSV_VALUE >> 8 ) & 0xff ) &&
            p[1] == (unsigned char)( ( MBEDTLS_SSL_FALLBACK_SCSV_VALUE      ) & 0xff ) )
        {
            MBEDTLS_SSL_DEBUG_MSG( 2, ( "received FALLBACK_SCSV" ) );

            if( ssl->minor_ver < ssl->conf->max_minor_ver )
            {
                MBEDTLS_SSL_DEBUG_MSG( 1, ( "inapropriate fallback" ) );

                mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_INAPROPRIATE_FALLBACK );

                return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
            }

            break;
        }
    }
#endif /* MBEDTLS_SSL_FALLBACK_SCSV */

#if defined(MBEDTLS_SSL_PROTO_TLS1_2) && \
    defined(MBEDTLS_KEY_EXCHANGE__WITH_CERT__ENABLED)

    /*
     * Try to fall back to default hash SHA1 if the client
     * hasn't provided any preferred signature-hash combinations.
     */
    if( sig_hash_alg_ext_present == 0 )
    {
        mbedtls_md_type_t md_default = MBEDTLS_MD_SHA1;

        if( mbedtls_ssl_check_sig_hash( ssl, md_default ) != 0 )
            md_default = MBEDTLS_MD_NONE;

        mbedtls_ssl_sig_hash_set_const_hash( &ssl->handshake->hash_algs, md_default );
    }

#endif /* MBEDTLS_SSL_PROTO_TLS1_2 &&
          MBEDTLS_KEY_EXCHANGE__WITH_CERT__ENABLED */

    /*
     * Check for TLS_EMPTY_RENEGOTIATION_INFO_SCSV
     */
    for( i = 0, p = buf + ciph_offset + 2; i < ciph_len; i += 2, p += 2 )
    {
        if( p[0] == 0 && p[1] == MBEDTLS_SSL_EMPTY_RENEGOTIATION_INFO )
        {
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "received TLS_EMPTY_RENEGOTIATION_INFO " ) );
#if defined(MBEDTLS_SSL_RENEGOTIATION)
            if( ssl->renego_status == MBEDTLS_SSL_RENEGOTIATION_IN_PROGRESS )
            {
                MBEDTLS_SSL_DEBUG_MSG( 1, ( "received RENEGOTIATION SCSV "
                                            "during renegotiation" ) );
                mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                                MBEDTLS_SSL_ALERT_MSG_HANDSHAKE_FAILURE );
                return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
            }
#endif
            ssl->secure_renegotiation = MBEDTLS_SSL_SECURE_RENEGOTIATION;
            break;
        }
    }

    /*
     * Renegotiation security checks
     */
    if( ssl->secure_renegotiation != MBEDTLS_SSL_SECURE_RENEGOTIATION &&
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
        return( MBEDTLS_ERR_SSL_BAD_HS_CLIENT_HELLO );
    }

    /*
     * Search for a matching ciphersuite
     * (At the end because we need information from the EC-based extensions
     * and certificate from the SNI callback triggered by the SNI extension.)
     */
    got_common_suite = 0;
    ciphersuites = ssl->conf->ciphersuite_list[ssl->minor_ver];
    ciphersuite_info = NULL;
#if defined(MBEDTLS_SSL_SRV_RESPECT_CLIENT_PREFERENCE)
    for( j = 0, p = buf + ciph_offset + 2; j < ciph_len; j += 2, p += 2 )
        for( i = 0; ciphersuites[i] != 0; i++ )
#else
    for( i = 0; ciphersuites[i] != 0; i++ )
        for( j = 0, p = buf + ciph_offset + 2; j < ciph_len; j += 2, p += 2 )
#endif
        {
            if( p[0] != ( ( ciphersuites[i] >> 8 ) & 0xFF ) ||
                p[1] != ( ( ciphersuites[i]      ) & 0xFF ) )
                continue;

            got_common_suite = 1;

            if( ( ret = ssl_ciphersuite_match( ssl, ciphersuites[i],
                                               &ciphersuite_info ) ) != 0 )
                return( ret );

            if( ciphersuite_info != NULL )
                goto have_ciphersuite;
        }

    if( got_common_suite )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "got ciphersuites in common, "
                            "but none of them usable" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_HANDSHAKE_FAILURE );
        return( MBEDTLS_ERR_SSL_NO_USABLE_CIPHERSUITE );
    }
    else
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "got no ciphersuites in common" ) );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_HANDSHAKE_FAILURE );
        return( MBEDTLS_ERR_SSL_NO_CIPHER_CHOSEN );
    }

have_ciphersuite:
    MBEDTLS_SSL_DEBUG_MSG( 2, ( "selected ciphersuite: %s", ciphersuite_info->name ) );

    ssl->session_negotiate->ciphersuite = ciphersuites[i];
    ssl->transform_negotiate->ciphersuite_info = ciphersuite_info;

    ssl->state++;

#if defined(MBEDTLS_SSL_PROTO_DTLS)
    if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM )
        mbedtls_ssl_recv_flight_completed( ssl );
#endif

    /* Debugging-only output for testsuite */
#if defined(MBEDTLS_DEBUG_C)                         && \
    defined(MBEDTLS_SSL_PROTO_TLS1_2)                && \
    defined(MBEDTLS_KEY_EXCHANGE__WITH_CERT__ENABLED)
    if( ssl->minor_ver == MBEDTLS_SSL_MINOR_VERSION_3 )
    {
        mbedtls_pk_type_t sig_alg = mbedtls_ssl_get_ciphersuite_sig_alg( ciphersuite_info );
        if( sig_alg != MBEDTLS_PK_NONE )
        {
            mbedtls_md_type_t md_alg = mbedtls_ssl_sig_hash_set_find( &ssl->handshake->hash_algs,
                                                                  sig_alg );
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello v3, signature_algorithm ext: %d",
                                        mbedtls_ssl_hash_from_md_alg( md_alg ) ) );
        }
        else
        {
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "no hash algorithm for signature algorithm "
                                        "%d - should not happen", sig_alg ) );
        }
    }
#endif

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= parse client hello" ) );

    return( 0 );
}
