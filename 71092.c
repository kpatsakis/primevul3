krb5_init_creds_set_fast_ap_armor_service(krb5_context context,
					  krb5_init_creds_context ctx,
					  krb5_const_principal armor_service)
{
    krb5_error_code ret;

    if (ctx->fast_state.armor_service)
	krb5_free_principal(context, ctx->fast_state.armor_service);
    if (armor_service) {
	ret = krb5_copy_principal(context, armor_service, &ctx->fast_state.armor_service);
	if (ret)
	    return ret;
    } else {
	ctx->fast_state.armor_service = NULL;
    }
    ctx->fast_state.flags |= KRB5_FAST_REQUIRED | KRB5_FAST_AP_ARMOR_SERVICE;
    return 0;
}
