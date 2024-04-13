krb5_init_creds_set_service(krb5_context context,
			    krb5_init_creds_context ctx,
			    const char *service)
{
    krb5_const_realm client_realm;
    krb5_principal principal;
    krb5_error_code ret;

    client_realm = krb5_principal_get_realm (context, ctx->cred.client);

    if (service) {
	ret = krb5_parse_name (context, service, &principal);
	if (ret)
	    return ret;
	krb5_principal_set_realm (context, principal, client_realm);
    } else {
	ret = krb5_make_principal(context, &principal,
				  client_realm, KRB5_TGS_NAME, client_realm,
				  NULL);
	if (ret)
	    return ret;
    }

    /*
     * This is for Windows RODC that are picky about what name type
     * the server principal have, and the really strange part is that
     * they are picky about the AS-REQ name type and not the TGS-REQ
     * later. Oh well.
     */

    if (krb5_principal_is_krbtgt(context, principal))
	krb5_principal_set_type(context, principal, KRB5_NT_SRV_INST);

    krb5_free_principal(context, ctx->cred.server);
    ctx->cred.server = principal;

    return 0;
}
