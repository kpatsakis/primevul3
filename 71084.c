krb5_get_init_creds_keyblock(krb5_context context,
			     krb5_creds *creds,
			     krb5_principal client,
			     krb5_keyblock *keyblock,
			     krb5_deltat start_time,
			     const char *in_tkt_service,
			     krb5_get_init_creds_opt *options)
{
    krb5_init_creds_context ctx;
    krb5_error_code ret;

    memset(creds, 0, sizeof(*creds));

    ret = krb5_init_creds_init(context, client, NULL, NULL, start_time, options, &ctx);
    if (ret)
	goto out;

    ret = krb5_init_creds_set_service(context, ctx, in_tkt_service);
    if (ret)
	goto out;

    ret = krb5_init_creds_set_keyblock(context, ctx, keyblock);
    if (ret)
	goto out;

    ret = krb5_init_creds_get(context, ctx);

    if (ret == 0)
        krb5_process_last_request(context, options, ctx);

 out:
    if (ret == 0)
	krb5_init_creds_get_creds(context, ctx, creds);

    if (ctx)
	krb5_init_creds_free(context, ctx);

    return ret;
}
