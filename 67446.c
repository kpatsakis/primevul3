static int ssl_parse_ecjpake_kkpp( mbedtls_ssl_context *ssl,
                                   const unsigned char *buf,
                                   size_t len )
{
    int ret;

    if( ssl->transform_negotiate->ciphersuite_info->key_exchange !=
        MBEDTLS_KEY_EXCHANGE_ECJPAKE )
    {
        MBEDTLS_SSL_DEBUG_MSG( 3, ( "skip ecjpake kkpp extension" ) );
        return( 0 );
    }

    /* If we got here, we no longer need our cached extension */
    mbedtls_free( ssl->handshake->ecjpake_cache );
    ssl->handshake->ecjpake_cache = NULL;
    ssl->handshake->ecjpake_cache_len = 0;

    if( ( ret = mbedtls_ecjpake_read_round_one( &ssl->handshake->ecjpake_ctx,
                                                buf, len ) ) != 0 )
    {
        MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ecjpake_read_round_one", ret );
        mbedtls_ssl_send_alert_message( ssl, MBEDTLS_SSL_ALERT_LEVEL_FATAL,
                                        MBEDTLS_SSL_ALERT_MSG_HANDSHAKE_FAILURE );
        return( ret );
    }

    return( 0 );
}
