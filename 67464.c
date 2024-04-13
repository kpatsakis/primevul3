static void ssl_write_encrypt_then_mac_ext( mbedtls_ssl_context *ssl,
                                       unsigned char *buf, size_t *olen )
{
    unsigned char *p = buf;
    const unsigned char *end = ssl->out_msg + MBEDTLS_SSL_MAX_CONTENT_LEN;

    *olen = 0;

    if( ssl->conf->encrypt_then_mac == MBEDTLS_SSL_ETM_DISABLED ||
        ssl->conf->max_minor_ver == MBEDTLS_SSL_MINOR_VERSION_0 )
    {
        return;
    }

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, adding encrypt_then_mac "
                        "extension" ) );

    if( end < p || (size_t)( end - p ) < 4 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "buffer too small" ) );
        return;
    }

    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_ENCRYPT_THEN_MAC >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_ENCRYPT_THEN_MAC      ) & 0xFF );

    *p++ = 0x00;
    *p++ = 0x00;

    *olen = 4;
}
