init_cred (krb5_context context,
	   krb5_creds *cred,
	   krb5_principal client,
	   krb5_deltat start_time,
	   krb5_get_init_creds_opt *options)
{
    krb5_error_code ret;
    int tmp;
    krb5_timestamp now;

    krb5_timeofday (context, &now);

    memset (cred, 0, sizeof(*cred));

    if (client)
	ret = krb5_copy_principal(context, client, &cred->client);
    else
	ret = krb5_get_default_principal(context, &cred->client);
    if (ret)
        goto out;

    if (start_time)
	cred->times.starttime  = now + start_time;

    if (options->flags & KRB5_GET_INIT_CREDS_OPT_TKT_LIFE)
	tmp = options->tkt_life;
    else
	tmp = KRB5_TKT_LIFETIME_DEFAULT;
    cred->times.endtime = now + tmp;

    if ((options->flags & KRB5_GET_INIT_CREDS_OPT_RENEW_LIFE)) {
	if (options->renew_life > 0)
	    tmp = options->renew_life;
	else
	    tmp = KRB5_TKT_RENEW_LIFETIME_DEFAULT;
	cred->times.renew_till = now + tmp;
    }

    return 0;

out:
    krb5_free_cred_contents (context, cred);
    return ret;
}
