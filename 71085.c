krb5_get_init_creds_keytab(krb5_context context,
			   krb5_creds *creds,
			   krb5_principal client,
			   krb5_keytab keytab,
			   krb5_deltat start_time,
			   const char *in_tkt_service,
			   krb5_get_init_creds_opt *options)
{
    krb5_init_creds_context ctx;
    krb5_keytab_entry ktent;
    krb5_error_code ret;

    memset(&ktent, 0, sizeof(ktent));
    memset(creds, 0, sizeof(*creds));

    if (strcmp(client->realm, "") == 0) {
        /*
         * Referral realm.  We have a keytab, so pick a realm by
         * matching in the keytab.
         */
        ret = krb5_kt_get_entry(context, keytab, client, 0, 0, &ktent);
        if (ret == 0)
            client = ktent.principal;
    }

    ret = krb5_init_creds_init(context, client, NULL, NULL, start_time, options, &ctx);
    if (ret)
	goto out;

    ret = krb5_init_creds_set_service(context, ctx, in_tkt_service);
    if (ret)
	goto out;

    ret = krb5_init_creds_set_keytab(context, ctx, keytab);
    if (ret)
	goto out;

    ret = krb5_init_creds_get(context, ctx);
    if (ret == 0)
        krb5_process_last_request(context, options, ctx);

 out:
    krb5_kt_free_entry(context, &ktent);
    if (ret == 0)
	krb5_init_creds_get_creds(context, ctx, creds);

    if (ctx)
	krb5_init_creds_free(context, ctx);

    return ret;
}
