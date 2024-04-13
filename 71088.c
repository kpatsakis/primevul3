krb5_init_creds_get(krb5_context context, krb5_init_creds_context ctx)
{
    krb5_sendto_ctx stctx = NULL;
    krb5_krbhst_info *hostinfo = NULL;
    krb5_error_code ret;
    krb5_data in, out;
    unsigned int flags = 0;

    krb5_data_zero(&in);
    krb5_data_zero(&out);

    ret = krb5_sendto_ctx_alloc(context, &stctx);
    if (ret)
	goto out;
    krb5_sendto_ctx_set_func(stctx, _krb5_kdc_retry, NULL);

    while (1) {
	flags = 0;
	ret = krb5_init_creds_step(context, ctx, &in, &out, hostinfo, &flags);
	krb5_data_free(&in);
	if (ret)
	    goto out;

	if ((flags & 1) == 0)
	    break;

	ret = krb5_sendto_context (context, stctx, &out,
				   ctx->cred.client->realm, &in);
    	if (ret)
	    goto out;

    }

 out:
    if (stctx)
	krb5_sendto_ctx_free(context, stctx);

    return ret;
}
