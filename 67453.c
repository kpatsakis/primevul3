static int ssl_parse_server_dh_params( mbedtls_ssl_context *ssl, unsigned char **p,
                                       unsigned char *end )
{
    int ret = MBEDTLS_ERR_SSL_FEATURE_UNAVAILABLE;

    /*
     * Ephemeral DH parameters:
     *
     * struct {
     *     opaque dh_p<1..2^16-1>;
     *     opaque dh_g<1..2^16-1>;
     *     opaque dh_Ys<1..2^16-1>;
     * } ServerDHParams;
     */
    if( ( ret = mbedtls_dhm_read_params( &ssl->handshake->dhm_ctx, p, end ) ) != 0 )
    {
        MBEDTLS_SSL_DEBUG_RET( 2, ( "mbedtls_dhm_read_params" ), ret );
        return( ret );
    }

    if( ssl->handshake->dhm_ctx.len * 8 < ssl->conf->dhm_min_bitlen )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "DHM prime too short: %d < %d",
                                    ssl->handshake->dhm_ctx.len * 8,
                                    ssl->conf->dhm_min_bitlen ) );
        return( MBEDTLS_ERR_SSL_BAD_HS_SERVER_KEY_EXCHANGE );
    }

    MBEDTLS_SSL_DEBUG_MPI( 3, "DHM: P ", &ssl->handshake->dhm_ctx.P  );
    MBEDTLS_SSL_DEBUG_MPI( 3, "DHM: G ", &ssl->handshake->dhm_ctx.G  );
    MBEDTLS_SSL_DEBUG_MPI( 3, "DHM: GY", &ssl->handshake->dhm_ctx.GY );

    return( ret );
}
