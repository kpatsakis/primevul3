static void ssl_write_ecjpake_kkpp_ext( mbedtls_ssl_context *ssl,
                                        unsigned char *buf,
                                        size_t *olen )
{
    int ret;
    unsigned char *p = buf;
    const unsigned char *end = ssl->out_msg + MBEDTLS_SSL_MAX_CONTENT_LEN;
    size_t kkpp_len;

    *olen = 0;

    /* Skip costly computation if not needed */
    if( ssl->transform_negotiate->ciphersuite_info->key_exchange !=
        MBEDTLS_KEY_EXCHANGE_ECJPAKE )
        return;

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "server hello, ecjpake kkpp extension" ) );

    if( end - p < 4 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "buffer too small" ) );
        return;
    }

    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_ECJPAKE_KKPP >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_ECJPAKE_KKPP      ) & 0xFF );

    ret = mbedtls_ecjpake_write_round_one( &ssl->handshake->ecjpake_ctx,
                                        p + 2, end - p - 2, &kkpp_len,
                                        ssl->conf->f_rng, ssl->conf->p_rng );
    if( ret != 0 )
    {
        MBEDTLS_SSL_DEBUG_RET( 1 , "mbedtls_ecjpake_write_round_one", ret );
        return;
    }

    *p++ = (unsigned char)( ( kkpp_len >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( kkpp_len      ) & 0xFF );

    *olen = kkpp_len + 4;
}
