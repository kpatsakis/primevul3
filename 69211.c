static int ssl_parse_session_ticket_ext( mbedtls_ssl_context *ssl,
                                         unsigned char *buf,
                                         size_t len )
{
    int ret;
    mbedtls_ssl_session session;

    mbedtls_ssl_session_init( &session );

    if( ssl->conf->f_ticket_parse == NULL ||
        ssl->conf->f_ticket_write == NULL )
    {
        return( 0 );
    }

    /* Remember the client asked us to send a new ticket */
    ssl->handshake->new_session_ticket = 1;

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "ticket length: %d", len ) );

    if( len == 0 )
        return( 0 );

#if defined(MBEDTLS_SSL_RENEGOTIATION)
    if( ssl->renego_status != MBEDTLS_SSL_INITIAL_HANDSHAKE )
    {
        MBEDTLS_SSL_DEBUG_MSG( 3, ( "ticket rejected: renegotiating" ) );
        return( 0 );
    }
#endif /* MBEDTLS_SSL_RENEGOTIATION */

    /*
     * Failures are ok: just ignore the ticket and proceed.
     */
    if( ( ret = ssl->conf->f_ticket_parse( ssl->conf->p_ticket, &session,
                                           buf, len ) ) != 0 )
    {
        mbedtls_ssl_session_free( &session );

        if( ret == MBEDTLS_ERR_SSL_INVALID_MAC )
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "ticket is not authentic" ) );
        else if( ret == MBEDTLS_ERR_SSL_SESSION_TICKET_EXPIRED )
            MBEDTLS_SSL_DEBUG_MSG( 3, ( "ticket is expired" ) );
        else
            MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ssl_ticket_parse", ret );

        return( 0 );
    }

    /*
     * Keep the session ID sent by the client, since we MUST send it back to
     * inform them we're accepting the ticket  (RFC 5077 section 3.4)
     */
    session.id_len = ssl->session_negotiate->id_len;
    memcpy( &session.id, ssl->session_negotiate->id, session.id_len );

    mbedtls_ssl_session_free( ssl->session_negotiate );
    memcpy( ssl->session_negotiate, &session, sizeof( mbedtls_ssl_session ) );

    /* Zeroize instead of free as we copied the content */
    mbedtls_zeroize( &session, sizeof( mbedtls_ssl_session ) );

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "session successfully restored from ticket" ) );

    ssl->handshake->resume = 1;

    /* Don't send a new ticket after all, this one is OK */
    ssl->handshake->new_session_ticket = 0;

    return( 0 );
}
