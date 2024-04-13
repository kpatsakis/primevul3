krb5_process_last_request(krb5_context context,
			  krb5_get_init_creds_opt *options,
			  krb5_init_creds_context ctx)
{
    krb5_const_realm realm;
    LastReq *lr;
    krb5_boolean reported = FALSE;
    krb5_timestamp sec;
    time_t t;
    size_t i;

    /*
     * First check if there is a API consumer.
     */

    realm = krb5_principal_get_realm (context, ctx->cred.client);
    lr = &ctx->enc_part.last_req;

    if (options && options->opt_private && options->opt_private->lr.func) {
	krb5_last_req_entry **lre;

	lre = calloc(lr->len + 1, sizeof(*lre));
	if (lre == NULL)
	    return krb5_enomem(context);
	for (i = 0; i < lr->len; i++) {
	    lre[i] = calloc(1, sizeof(*lre[i]));
	    if (lre[i] == NULL)
		break;
	    lre[i]->lr_type = lr->val[i].lr_type;
	    lre[i]->value = lr->val[i].lr_value;
	}

	(*options->opt_private->lr.func)(context, lre,
					 options->opt_private->lr.ctx);

	for (i = 0; i < lr->len; i++)
	    free(lre[i]);
	free(lre);
    }

    /*
     * Now check if we should prompt the user
     */

    if (ctx->prompter == NULL)
        return 0;

    krb5_timeofday (context, &sec);

    t = sec + get_config_time (context,
			       realm,
			       "warn_pwexpire",
			       7 * 24 * 60 * 60);

    for (i = 0; i < lr->len; ++i) {
	if (lr->val[i].lr_value <= t) {
	    switch (lr->val[i].lr_type) {
	    case LR_PW_EXPTIME :
		report_expiration(context, ctx->prompter,
				  ctx->prompter_data,
				  "Your password will expire at ",
				  lr->val[i].lr_value);
		reported = TRUE;
		break;
	    case LR_ACCT_EXPTIME :
		report_expiration(context, ctx->prompter,
				  ctx->prompter_data,
				  "Your account will expire at ",
				  lr->val[i].lr_value);
		reported = TRUE;
		break;
            default:
                break;
	    }
	}
    }

    if (!reported
	&& ctx->enc_part.key_expiration
	&& *ctx->enc_part.key_expiration <= t) {
        report_expiration(context, ctx->prompter,
			  ctx->prompter_data,
			  "Your password/account will expire at ",
			  *ctx->enc_part.key_expiration);
    }
    return 0;
}
