_krb5_make_fast_ap_fxarmor(krb5_context context,
			   krb5_ccache armor_ccache,
			   krb5_data *armor_value,
			   krb5_keyblock *armor_key,
			   krb5_crypto *armor_crypto)
{
    krb5_auth_context auth_context = NULL;
    krb5_creds cred, *credp = NULL;
    krb5_error_code ret;
    krb5_data empty;

    krb5_data_zero(&empty);

    memset(&cred, 0, sizeof(cred));

    ret = krb5_auth_con_init (context, &auth_context);
    if (ret)
	goto out;
    
    ret = krb5_cc_get_principal(context, armor_ccache, &cred.client);
    if (ret)
	goto out;
    
    ret = krb5_make_principal(context, &cred.server,
			      cred.client->realm,
			      KRB5_TGS_NAME,
			      cred.client->realm,
			      NULL);
    if (ret) {
	krb5_free_principal(context, cred.client);
	goto out;
    }
    
    ret = krb5_get_credentials(context, 0, armor_ccache, &cred, &credp);
    krb5_free_principal(context, cred.server);
    krb5_free_principal(context, cred.client);
    if (ret)
	goto out;
    
    ret = krb5_auth_con_add_AuthorizationData(context, auth_context, KRB5_PADATA_FX_FAST_ARMOR, &empty);
    if (ret)
	goto out;

    ret = krb5_mk_req_extended(context,
			       &auth_context,
			       AP_OPTS_USE_SUBKEY,
			       NULL,
			       credp,
			       armor_value);
    krb5_free_creds(context, credp);
    if (ret)
	goto out;
    
    ret = _krb5_fast_armor_key(context,
			       auth_context->local_subkey,
			       auth_context->keyblock,
			       armor_key,
			       armor_crypto);
    if (ret)
	goto out;

 out:
    krb5_auth_con_free(context, auth_context);
    return ret;
}
