static int ssl_write_certificate_request( mbedtls_ssl_context *ssl )
{
    int ret = MBEDTLS_ERR_SSL_FEATURE_UNAVAILABLE;
    const mbedtls_ssl_ciphersuite_t *ciphersuite_info =
        ssl->transform_negotiate->ciphersuite_info;
    size_t dn_size, total_dn_size; /* excluding length bytes */
    size_t ct_len, sa_len; /* including length bytes */
    unsigned char *buf, *p;
    const unsigned char * const end = ssl->out_msg + MBEDTLS_SSL_MAX_CONTENT_LEN;
    const mbedtls_x509_crt *crt;
    int authmode;

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "=> write certificate request" ) );

    ssl->state++;

#if defined(MBEDTLS_SSL_SERVER_NAME_INDICATION)
    if( ssl->handshake->sni_authmode != MBEDTLS_SSL_VERIFY_UNSET )
        authmode = ssl->handshake->sni_authmode;
    else
#endif
        authmode = ssl->conf->authmode;

    if( ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_PSK ||
        ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_RSA_PSK ||
        ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_DHE_PSK ||
        ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_ECDHE_PSK ||
        ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_ECJPAKE ||
        authmode == MBEDTLS_SSL_VERIFY_NONE )
    {
        MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= skip write certificate request" ) );
        return( 0 );
    }

    /*
     *     0  .   0   handshake type
     *     1  .   3   handshake length
     *     4  .   4   cert type count
     *     5  .. m-1  cert types
     *     m  .. m+1  sig alg length (TLS 1.2 only)
     *    m+1 .. n-1  SignatureAndHashAlgorithms (TLS 1.2 only)
     *     n  .. n+1  length of all DNs
     *    n+2 .. n+3  length of DN 1
     *    n+4 .. ...  Distinguished Name #1
     *    ... .. ...  length of DN 2, etc.
     */
    buf = ssl->out_msg;
    p = buf + 4;

    /*
     * Supported certificate types
     *
     *     ClientCertificateType certificate_types<1..2^8-1>;
     *     enum { (255) } ClientCertificateType;
     */
    ct_len = 0;

#if defined(MBEDTLS_RSA_C)
    p[1 + ct_len++] = MBEDTLS_SSL_CERT_TYPE_RSA_SIGN;
#endif
#if defined(MBEDTLS_ECDSA_C)
    p[1 + ct_len++] = MBEDTLS_SSL_CERT_TYPE_ECDSA_SIGN;
#endif

    p[0] = (unsigned char) ct_len++;
    p += ct_len;

    sa_len = 0;
#if defined(MBEDTLS_SSL_PROTO_TLS1_2)
    /*
     * Add signature_algorithms for verify (TLS 1.2)
     *
     *     SignatureAndHashAlgorithm supported_signature_algorithms<2..2^16-2>;
     *
     *     struct {
     *           HashAlgorithm hash;
     *           SignatureAlgorithm signature;
     *     } SignatureAndHashAlgorithm;
     *
     *     enum { (255) } HashAlgorithm;
     *     enum { (255) } SignatureAlgorithm;
     */
    if( ssl->minor_ver == MBEDTLS_SSL_MINOR_VERSION_3 )
    {
        const int *cur;

        /*
         * Supported signature algorithms
         */
        for( cur = ssl->conf->sig_hashes; *cur != MBEDTLS_MD_NONE; cur++ )
        {
            unsigned char hash = mbedtls_ssl_hash_from_md_alg( *cur );

            if( MBEDTLS_SSL_HASH_NONE == hash || mbedtls_ssl_set_calc_verify_md( ssl, hash ) )
                continue;

#if defined(MBEDTLS_RSA_C)
            p[2 + sa_len++] = hash;
            p[2 + sa_len++] = MBEDTLS_SSL_SIG_RSA;
#endif
#if defined(MBEDTLS_ECDSA_C)
            p[2 + sa_len++] = hash;
            p[2 + sa_len++] = MBEDTLS_SSL_SIG_ECDSA;
#endif
        }

        p[0] = (unsigned char)( sa_len >> 8 );
        p[1] = (unsigned char)( sa_len      );
        sa_len += 2;
        p += sa_len;
    }
#endif /* MBEDTLS_SSL_PROTO_TLS1_2 */

    /*
     * DistinguishedName certificate_authorities<0..2^16-1>;
     * opaque DistinguishedName<1..2^16-1>;
     */
    p += 2;

    total_dn_size = 0;

    if( ssl->conf->cert_req_ca_list ==  MBEDTLS_SSL_CERT_REQ_CA_LIST_ENABLED )
    {
#if defined(MBEDTLS_SSL_SERVER_NAME_INDICATION)
        if( ssl->handshake->sni_ca_chain != NULL )
            crt = ssl->handshake->sni_ca_chain;
        else
#endif
            crt = ssl->conf->ca_chain;

        while( crt != NULL && crt->version != 0 )
        {
            dn_size = crt->subject_raw.len;

            if( end < p ||
                (size_t)( end - p ) < dn_size ||
                (size_t)( end - p ) < 2 + dn_size )
            {
                MBEDTLS_SSL_DEBUG_MSG( 1, ( "skipping CAs: buffer too short" ) );
                break;
            }

            *p++ = (unsigned char)( dn_size >> 8 );
            *p++ = (unsigned char)( dn_size      );
            memcpy( p, crt->subject_raw.p, dn_size );
            p += dn_size;

            MBEDTLS_SSL_DEBUG_BUF( 3, "requested DN", p - dn_size, dn_size );

            total_dn_size += 2 + dn_size;
            crt = crt->next;
        }
    }

    ssl->out_msglen  = p - buf;
    ssl->out_msgtype = MBEDTLS_SSL_MSG_HANDSHAKE;
    ssl->out_msg[0]  = MBEDTLS_SSL_HS_CERTIFICATE_REQUEST;
    ssl->out_msg[4 + ct_len + sa_len] = (unsigned char)( total_dn_size  >> 8 );
    ssl->out_msg[5 + ct_len + sa_len] = (unsigned char)( total_dn_size       );

    ret = mbedtls_ssl_write_record( ssl );

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= write certificate request" ) );

    return( ret );
}
