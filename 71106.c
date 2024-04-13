process_pa_data_to_md(krb5_context context,
		      const krb5_creds *creds,
		      const AS_REQ *a,
		      krb5_get_init_creds_ctx *ctx,
		      METHOD_DATA *in_md,
		      METHOD_DATA **out_md,
		      krb5_prompter_fct prompter,
		      void *prompter_data)
{
    krb5_error_code ret;

    ALLOC(*out_md, 1);
    if (*out_md == NULL)
	return krb5_enomem(context);

    (*out_md)->len = 0;
    (*out_md)->val = NULL;

    if (_krb5_have_debug(context, 5)) {
	unsigned i;
	_krb5_debug(context, 5, "KDC send %d patypes", in_md->len);
	for (i = 0; i < in_md->len; i++)
	    _krb5_debug(context, 5, "KDC send PA-DATA type: %d", in_md->val[i].padata_type);
    }

    /*
     * Make sure we don't sent both ENC-TS and PK-INIT pa data, no
     * need to expose our password protecting our PKCS12 key.
     */

    if (ctx->pk_init_ctx) {

 	_krb5_debug(context, 5, "krb5_get_init_creds: "
		    "prepareing PKINIT padata (%s)",
 		    (ctx->used_pa_types & USED_PKINIT_W2K) ? "win2k" : "ietf");

 	if (ctx->used_pa_types & USED_PKINIT_W2K) {
 	    krb5_set_error_message(context, KRB5_GET_IN_TKT_LOOP,
 				   "Already tried pkinit, looping");
 	    return KRB5_GET_IN_TKT_LOOP;
 	}

	ret = pa_data_to_md_pkinit(context, a, creds->client,
				   (ctx->used_pa_types & USED_PKINIT),
				   ctx, *out_md);
	if (ret)
	    return ret;

	if (ctx->used_pa_types & USED_PKINIT)
	    ctx->used_pa_types |= USED_PKINIT_W2K;
 	else
 	    ctx->used_pa_types |= USED_PKINIT;

    } else if (in_md->len != 0) {
	struct pa_info_data *paid, *ppaid;
 	unsigned flag;

	paid = calloc(1, sizeof(*paid));
        if (paid == NULL)
            return krb5_enomem(context);

	paid->etype = KRB5_ENCTYPE_NULL;
	ppaid = process_pa_info(context, creds->client, a, paid, in_md);

 	if (ppaid)
 	    flag = USED_ENC_TS_INFO;
 	else
 	    flag = USED_ENC_TS_GUESS;

 	if (ctx->used_pa_types & flag) {
 	    if (ppaid)
 		free_paid(context, ppaid);
            free(paid);
 	    krb5_set_error_message(context, KRB5_GET_IN_TKT_LOOP,
 				   "Already tried ENC-TS-%s, looping",
 				   flag == USED_ENC_TS_INFO ? "info" : "guess");
 	    return KRB5_GET_IN_TKT_LOOP;
 	}

	pa_data_to_md_ts_enc(context, a, creds->client, ctx, ppaid, *out_md);

	ctx->used_pa_types |= flag;

	if (ppaid) {
	    if (ctx->ppaid) {
		free_paid(context, ctx->ppaid);
		free(ctx->ppaid);
	    }
	    ctx->ppaid = ppaid;
	} else
	    free(paid);
    }

    pa_data_add_pac_request(context, ctx, *out_md);

    if ((ctx->fast_state.flags & KRB5_FAST_DISABLED) == 0) {
 	ret = krb5_padata_add(context, *out_md, KRB5_PADATA_REQ_ENC_PA_REP, NULL, 0);
 	if (ret)
 	    return ret;
    }

    if ((*out_md)->len == 0) {
	free(*out_md);
	*out_md = NULL;
    }

    return 0;
}
