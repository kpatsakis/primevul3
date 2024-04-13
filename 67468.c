static void ssl_write_max_fragment_length_ext( mbedtls_ssl_context *ssl,
                                               unsigned char *buf,
                                               size_t *olen )
{
    unsigned char *p = buf;
    const unsigned char *end = ssl->out_msg + MBEDTLS_SSL_MAX_CONTENT_LEN;

    *olen = 0;

    if( ssl->conf->mfl_code == MBEDTLS_SSL_MAX_FRAG_LEN_NONE ) {
        return;
    }

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, adding max_fragment_length extension" ) );

    if( end < p || (size_t)( end - p ) < 5 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "buffer too small" ) );
        return;
    }

    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_MAX_FRAGMENT_LENGTH >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_MAX_FRAGMENT_LENGTH      ) & 0xFF );

    *p++ = 0x00;
    *p++ = 1;

    *p++ = ssl->conf->mfl_code;

    *olen = 5;
}
