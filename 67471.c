static void ssl_write_supported_elliptic_curves_ext( mbedtls_ssl_context *ssl,
                                                     unsigned char *buf,
                                                     size_t *olen )
{
    unsigned char *p = buf;
    const unsigned char *end = ssl->out_msg + MBEDTLS_SSL_MAX_CONTENT_LEN;
    unsigned char *elliptic_curve_list = p + 6;
    size_t elliptic_curve_len = 0;
    const mbedtls_ecp_curve_info *info;
#if defined(MBEDTLS_ECP_C)
    const mbedtls_ecp_group_id *grp_id;
#else
    ((void) ssl);
#endif

    *olen = 0;

    MBEDTLS_SSL_DEBUG_MSG( 3, ( "client hello, adding supported_elliptic_curves extension" ) );

#if defined(MBEDTLS_ECP_C)
    for( grp_id = ssl->conf->curve_list; *grp_id != MBEDTLS_ECP_DP_NONE; grp_id++ )
#else
    for( info = mbedtls_ecp_curve_list(); info->grp_id != MBEDTLS_ECP_DP_NONE; info++ )
#endif
    {
#if defined(MBEDTLS_ECP_C)
        info = mbedtls_ecp_curve_info_from_grp_id( *grp_id );
#endif
        if( info == NULL )
        {
            MBEDTLS_SSL_DEBUG_MSG( 1, ( "invalid curve in ssl configuration" ) );
            return;
        }

        elliptic_curve_len += 2;
    }

    if( end < p || (size_t)( end - p ) < 6 + elliptic_curve_len )
    {
        MBEDTLS_SSL_DEBUG_MSG( 1, ( "buffer too small" ) );
        return;
    }

    elliptic_curve_len = 0;

#if defined(MBEDTLS_ECP_C)
    for( grp_id = ssl->conf->curve_list; *grp_id != MBEDTLS_ECP_DP_NONE; grp_id++ )
#else
    for( info = mbedtls_ecp_curve_list(); info->grp_id != MBEDTLS_ECP_DP_NONE; info++ )
#endif
    {
#if defined(MBEDTLS_ECP_C)
        info = mbedtls_ecp_curve_info_from_grp_id( *grp_id );
#endif
        elliptic_curve_list[elliptic_curve_len++] = info->tls_id >> 8;
        elliptic_curve_list[elliptic_curve_len++] = info->tls_id & 0xFF;
    }

    if( elliptic_curve_len == 0 )
        return;

    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_SUPPORTED_ELLIPTIC_CURVES >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( MBEDTLS_TLS_EXT_SUPPORTED_ELLIPTIC_CURVES      ) & 0xFF );

    *p++ = (unsigned char)( ( ( elliptic_curve_len + 2 ) >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( ( elliptic_curve_len + 2 )      ) & 0xFF );

    *p++ = (unsigned char)( ( ( elliptic_curve_len     ) >> 8 ) & 0xFF );
    *p++ = (unsigned char)( ( ( elliptic_curve_len     )      ) & 0xFF );

    *olen = 6 + elliptic_curve_len;
}
