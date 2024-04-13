static void ssl_write_session_ticket_ext( mbedtls_ssl_context *ssl,
                                          unsigned char *buf, size_t *olen )
{
    unsigned char *p = buf;
    const unsigned char *end = ssl->out_msg + MBEDTLS_SSL_MAX_CONTENT_LEN;
    size_t tlen = ssl->session_negotiate->ticket_len;

    *olen = 0;

    if( ssl->conf->session_tickets == MBEDTLS_SSL_SESSION_TICKETS_DISABLED )
    {
        return;
    }

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, adding session ticket extension" ) );

    if( end < p || (size_t)( end - p ) < 4 + tlen )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "buffer too small" ) );
        return;
    }

    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_SESSION_TICKET >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_SESSION_TICKET      ) & 0xFF );

    *p++ = (unsigned char)( ( tlen >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( tlen      ) & 0xFF );

    *olen = 4;

    if( ssl->session_negotiate->ticket == NULL || tlen == 0 )
    {
        return;
    }

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "sending session ticket of length %d", tlen ) );

    memcpy( p, ssl->session_negotiate->ticket, tlen );

    *olen += tlen;
}
