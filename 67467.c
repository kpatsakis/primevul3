static void ssl_write_hostname_ext( mbedtls_ssl_context *ssl,
                                    unsigned char *buf,
                                    size_t *olen )
{
    unsigned char *p = buf;
    const unsigned char *end = ssl->out_msg + MBEDTLS_SSL_MAX_CONTENT_LEN;
    size_t hostname_len;

    *olen = 0;

    if( ssl->hostname == NULL )
        return;

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, adding server name extension: %s",
                   ssl->hostname ) );

    hostname_len = strlen( ssl->hostname );

    if( end < p || (size_t)( end - p ) < hostname_len + 9 )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "buffer too small" ) );
        return;
    }

    /*
     * Sect. 3, RFC 6066 (TLS Extensions Definitions)
     *
     * In order to provide any of the server names, clients MAY include an
     * extension of type "server_name" in the (extended) client hello. The
     * "extension_data" field of this extension SHALL contain
     * "ServerNameList" where:
     *
     * struct {
     *     NameType name_type;
     *     select (name_type) {
     *         case host_name: HostName;
     *     } name;
     * } ServerName;
     *
     * enum {
     *     host_name(0), (255)
     * } NameType;
     *
     * opaque HostName<1..2^16-1>;
     *
     * struct {
     *     ServerName server_name_list<1..2^16-1>
     * } ServerNameList;
     *
     */
    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_SERVERNAME >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_SERVERNAME      ) & 0xFF );

    *p++ = (unsigned char)( ( (hostname_len + 5) >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( (hostname_len + 5)      ) & 0xFF );

    *p++ = (unsigned char)( ( (hostname_len + 3) >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( (hostname_len + 3)      ) & 0xFF );

    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_SERVERNAME_HOSTNAME ) & 0xFF );
    *p++ = (unsigned char)( ( hostname_len >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( hostname_len      ) & 0xFF );

    memcpy( p, ssl->hostname, hostname_len );

    *olen = hostname_len + 9;
}
