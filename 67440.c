int mbedtls_ssl_handshake_client_step( mbedtls_ssl_context *ssl )
{
    int ret = 0;

    if( ssl->state == MBEDTLS_SSL_HANDSHAKE_OVER || ssl->handshake == NULL )
        return( MBEDTLS_ERR_SSL_BAD_INPUT_DATA );

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "client state: %d", ssl->state ) );

    if( ( ret = mbedtls_ssl_flush_output( ssl ) ) != 0 )
        return( ret );

#if defined(MBEDTLS_SSL_PROTO_DTLS)
    if( ssl->conf->transport == MBEDTLS_SSL_TRANSPORT_DATAGRAM &&
        ssl->handshake->retransmit_state == MBEDTLS_SSL_RETRANS_SENDING )
    {
        if( ( ret = mbedtls_ssl_resend( ssl ) ) != 0 )
            return( ret );
    }
#endif

    /* Change state now, so that it is right in mbedtls_ssl_read_record(), used
     * by DTLS for dropping out-of-sequence ChangeCipherSpec records */
#if defined(MBEDTLS_SSL_SESSION_TICKETS)
    if( ssl->state == MBEDTLS_SSL_SERVER_CHANGE_CIPHER_SPEC &&
        ssl->handshake->new_session_ticket != 0 )
    {
        ssl->state = MBEDTLS_SSL_SERVER_NEW_SESSION_TICKET;
    }
#endif

    switch( ssl->state )
    {
        case MBEDTLS_SSL_HELLO_REQUEST:
            ssl->state = MBEDTLS_SSL_CLIENT_HELLO;
            break;

       /*
        *  ==>   ClientHello
        */
       case MBEDTLS_SSL_CLIENT_HELLO:
           ret = ssl_write_client_hello( ssl );
           break;

       /*
        *  <==   ServerHello
        *        Certificate
        *      ( ServerKeyExchange  )
        *      ( CertificateRequest )
        *        ServerHelloDone
        */
       case MBEDTLS_SSL_SERVER_HELLO:
           ret = ssl_parse_server_hello( ssl );
           break;

       case MBEDTLS_SSL_SERVER_CERTIFICATE:
           ret = mbedtls_ssl_parse_certificate( ssl );
           break;

       case MBEDTLS_SSL_SERVER_KEY_EXCHANGE:
           ret = ssl_parse_server_key_exchange( ssl );
           break;

       case MBEDTLS_SSL_CERTIFICATE_REQUEST:
           ret = ssl_parse_certificate_request( ssl );
           break;

       case MBEDTLS_SSL_SERVER_HELLO_DONE:
           ret = ssl_parse_server_hello_done( ssl );
           break;

       /*
        *  ==> ( Certificate/Alert  )
        *        ClientKeyExchange
        *      ( CertificateVerify  )
        *        ChangeCipherSpec
        *        Finished
        */
       case MBEDTLS_SSL_CLIENT_CERTIFICATE:
           ret = mbedtls_ssl_write_certificate( ssl );
           break;

       case MBEDTLS_SSL_CLIENT_KEY_EXCHANGE:
           ret = ssl_write_client_key_exchange( ssl );
           break;

       case MBEDTLS_SSL_CERTIFICATE_VERIFY:
           ret = ssl_write_certificate_verify( ssl );
           break;

       case MBEDTLS_SSL_CLIENT_CHANGE_CIPHER_SPEC:
           ret = mbedtls_ssl_write_change_cipher_spec( ssl );
           break;

       case MBEDTLS_SSL_CLIENT_FINISHED:
           ret = mbedtls_ssl_write_finished( ssl );
           break;

       /*
        *  <==   ( NewSessionTicket )
        *        ChangeCipherSpec
        *        Finished
        */
#if defined(MBEDTLS_SSL_SESSION_TICKETS)
       case MBEDTLS_SSL_SERVER_NEW_SESSION_TICKET:
           ret = ssl_parse_new_session_ticket( ssl );
           break;
#endif

       case MBEDTLS_SSL_SERVER_CHANGE_CIPHER_SPEC:
           ret = mbedtls_ssl_parse_change_cipher_spec( ssl );
           break;

       case MBEDTLS_SSL_SERVER_FINISHED:
           ret = mbedtls_ssl_parse_finished( ssl );
           break;

       case MBEDTLS_SSL_FLUSH_BUFFERS:
           MBEDTLS_SSL_DEBUG_MSG( 2, ( "handshake: done" ) );
           ssl->state = MBEDTLS_SSL_HANDSHAKE_WRAPUP;
           break;

       case MBEDTLS_SSL_HANDSHAKE_WRAPUP:
           mbedtls_ssl_handshake_wrapup( ssl );
           break;

       default:
           MBEDTLS_SSL_DEBUG_MSG( 1, ( "invalid state %d", ssl->state ) );
           return( MBEDTLS_ERR_SSL_BAD_INPUT_DATA );
   }

    return( ret );
}
