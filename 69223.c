static int ssl_write_hello_verify_request( mbedtls_ssl_context *ssl )
{
    int ret;
    unsigned char *p = ssl->out_msg + 4;
    unsigned char *cookie_len_byte;

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "=> write hello verify request" ) );

    /*
     * struct {
     *   ProtocolVersion server_version;
     *   opaque cookie<0..2^8-1>;
     * } HelloVerifyRequest;
     */

    /* The RFC is not clear on this point, but sending the actual negotiated
     * version looks like the most interoperable thing to do. */
    mbedtls_ssl_write_version( ssl->major_ver, ssl->minor_ver,
                       ssl->conf->transport, p );
    MBEDTLS_SSL_DEBUG_BUF( 3, "server version", p, 2 );
    p += 2;

    /* If we get here, f_cookie_check is not null */
    if( ssl->conf->f_cookie_write == NULL )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "inconsistent cookie callbacks" ) );
        return( MBEDTLS_ERR_SSL_INTERNAL_ERROR );
    }

    /* Skip length byte until we know the length */
    cookie_len_byte = p++;

    if( ( ret = ssl->conf->f_cookie_write( ssl->conf->p_cookie,
                                     &p, ssl->out_buf + MBEDTLS_SSL_BUFFER_LEN,
                                     ssl->cli_id, ssl->cli_id_len ) ) != 0 )
    {
        MBEDTLS_SSL_DEBUG_RET( 1, "f_cookie_write", ret );
        return( ret );
    }

    *cookie_len_byte = (unsigned char)( p - ( cookie_len_byte + 1 ) );

    MBEDTLS_SSL_DEBUG_BUF( 3, "cookie sent", cookie_len_byte + 1, *cookie_len_byte );

    ssl->out_msglen  = p - ssl->out_msg;
    ssl->out_msgtype = MBEDTLS_SSL_MSG_HANDSHAKE;
    ssl->out_msg[0]  = MBEDTLS_SSL_HS_HELLO_VERIFY_REQUEST;

    ssl->state = MBEDTLS_SSL_SERVER_HELLO_VERIFY_REQUEST_SENT;

    if( ( ret = mbedtls_ssl_write_record( ssl ) ) != 0 )
    {
        MBEDTLS_SSL_DEBUG_RET( 1, "mbedtls_ssl_write_record", ret );
        return( ret );
    }

    MBEDTLS_SSL_DEBUG_MSG( 2, ( "<= write hello verify request" ) );

    return( 0 );
}
