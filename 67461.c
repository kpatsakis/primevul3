static int ssl_write_certificate_verify( mbedtls_ssl_context *ssl )
{
    int ret = MBEDTLS_ERR_SSL_FEATURE_UNAVAILABLE;
    const mbedtls_ssl_ciphersuite_t *ciphersuite_info =
        ssl->transform_negotiate->ciphersuite_info;
    size_t n = 0, offset = 0;
    unsigned char hash[48];
    unsigned char *hash_start = hash;
    mbedtls_md_type_t md_alg = MBEDTLS_MD_NONE;
    unsigned int hashlen;

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "=> write certificate verify" ) );

    if( ( ret = mbedtls_ssl_derive_keys( ssl ) ) != 0 )
    {
        MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ssl_derive_keys", ret );
        return( ret );
    }

    if( ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_PSK ||
        ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_RSA_PSK ||
        ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_ECDHE_PSK ||
        ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_DHE_PSK ||
        ciphersuite_info->key_exchange == MBEDTLS_KEY_EXCHANGE_ECJPAKE )
    {
        MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= skip write certificate verify" ) );
        ssl->state++;
        return( 0 );
    }

    if( ssl->client_auth == 0 || mbedtls_ssl_own_cert( ssl ) == NULL )
    {
        MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= skip write certificate verify" ) );
        ssl->state++;
        return( 0 );
    }

    if( mbedtls_ssl_own_key( ssl ) == NULL )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "got no private key for certificate" ) );
        return( MBEDTLS_ERR_SSL_PRIVATE_KEY_REQUIRED );
    }

    /*
     * Make an RSA signature of the handshake digests
     */
    ssl->handshake->calc_verify( ssl, hash );

#if defined(MBEDTLS_SSL_PROTO_SSL3) || defined(MBEDTLS_SSL_PROTO_TLS1) || \
    defined(MBEDTLS_SSL_PROTO_TLS1_1)
    if( ssl->minor_ver != MBEDTLS_SSL_MINOR_VERSION_3 )
    {
        /*
         * digitally-signed struct {
         *     opaque md5_hash[16];
         *     opaque sha_hash[20];
         * };
         *
         * md5_hash
         *     MD5(handshake_messages);
         *
         * sha_hash
         *     SHA(handshake_messages);
         */
        hashlen = 36;
        md_alg = MBEDTLS_MD_NONE;

        /*
         * For ECDSA, default hash is SHA-1 only
         */
        if( mbedtls_pk_can_do( mbedtls_ssl_own_key( ssl ), MBEDTLS_PK_ECDSA ) )
        {
            hash_start += 16;
            hashlen -= 16;
            md_alg = MBEDTLS_MD_SHA1;
        }
    }
    else
#endif /* MBEDTLS_SSL_PROTO_SSL3 || MBEDTLS_SSL_PROTO_TLS1 || \
          MBEDTLS_SSL_PROTO_TLS1_1 */
#if defined(MBEDTLS_SSL_PROTO_TLS1_2)
    if( ssl->minor_ver == MBEDTLS_SSL_MINOR_VERSION_3 )
    {
        /*
         * digitally-signed struct {
         *     opaque handshake_messages[handshake_messages_length];
         * };
         *
         * Taking shortcut here. We assume that the server always allows the
         * PRF Hash function and has sent it in the allowed signature
         * algorithms list received in the Certificate Request message.
         *
         * Until we encounter a server that does not, we will take this
         * shortcut.
         *
         * Reason: Otherwise we should have running hashes for SHA512 and SHA224
         *         in order to satisfy 'weird' needs from the server side.
         */
        if( ssl->transform_negotiate->ciphersuite_info->mac ==
            MBEDTLS_MD_SHA384 )
        {
            md_alg = MBEDTLS_MD_SHA384;
            ssl->out_msg[4] = MBEDTLS_SSL_HASH_SHA384;
        }
        else
        {
            md_alg = MBEDTLS_MD_SHA256;
            ssl->out_msg[4] = MBEDTLS_SSL_HASH_SHA256;
        }
        ssl->out_msg[5] = mbedtls_ssl_sig_from_pk( mbedtls_ssl_own_key( ssl ) );

        /* Info from md_alg will be used instead */
        hashlen = 0;
        offset = 2;
    }
    else
#endif /* MBEDTLS_SSL_PROTO_TLS1_2 */
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "should never happen" ) );
        return( MBEDTLS_ERR_SSL_INTERNAL_ERROR );
    }

    if( ( ret = mbedtls_pk_sign( mbedtls_ssl_own_key( ssl ), md_alg, hash_start, hashlen,
                         ssl->out_msg + 6 + offset, &n,
                         ssl->conf->f_rng, ssl->conf->p_rng ) ) != 0 )
    {
        MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_pk_sign", ret );
        return( ret );
    }

    ssl->out_msg[4 + offset] = (unsigned char)( n >> 8 );
    ssl->out_msg[5 + offset] = (unsigned char)( n      );

    ssl->out_msglen  = 6 + n + offset;
    ssl->out_msgtype = MBEDTLS_SSL_MSG_HANDSHAKE;
    ssl->out_msg[0]  = MBEDTLS_SSL_HS_CERTIFICATE_VERIFY;

    ssl->state++;

    if( ( ret = mbedtls_ssl_write_record( ssl ) ) != 0 )
    {
        MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ssl_write_record", ret );
        return( ret );
    }

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= write certificate verify" ) );

    return( ret );
}
