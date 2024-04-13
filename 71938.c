char *am_ecp_service_options_str(apr_pool_t *pool, ECPServiceOptions options)
{
    apr_array_header_t *names = apr_array_make(pool, 4, sizeof(const char *));

    if (options & ECP_SERVICE_OPTION_CHANNEL_BINDING) {
        APR_ARRAY_PUSH(names, const char *) = "channel-binding";
        options &= ~ECP_SERVICE_OPTION_CHANNEL_BINDING;
    }

    if (options & ECP_SERVICE_OPTION_HOLDER_OF_KEY) {
        APR_ARRAY_PUSH(names, const char *) = "holder-of-key";
        options &= ~ECP_SERVICE_OPTION_HOLDER_OF_KEY;
    }

    if (options & ECP_SERVICE_OPTION_WANT_AUTHN_SIGNED) {
        APR_ARRAY_PUSH(names, const char *) = "want-authn-signed";
        options &= ~ECP_SERVICE_OPTION_WANT_AUTHN_SIGNED;
    }

    if (options & ECP_SERVICE_OPTION_DELEGATION) {
        APR_ARRAY_PUSH(names, const char *) = "delegation";
        options &= ~ECP_SERVICE_OPTION_DELEGATION;
    }

    if (options) {
        APR_ARRAY_PUSH(names, const char *) =
            apr_psprintf(pool, "(unknown bits = %#x)", options);
    }

    return apr_array_pstrcat(pool, names, ',');
}
