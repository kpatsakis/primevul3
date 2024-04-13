process_pa_data_to_key(krb5_context context,
		       krb5_get_init_creds_ctx *ctx,
		       krb5_creds *creds,
		       AS_REQ *a,
		       AS_REP *rep,
		       const krb5_krbhst_info *hi,
		       krb5_keyblock **key)
{
    struct pa_info_data paid, *ppaid = NULL;
    krb5_error_code ret;
    krb5_enctype etype;
    PA_DATA *pa;

    memset(&paid, 0, sizeof(paid));

    etype = rep->enc_part.etype;

    if (rep->padata) {
	paid.etype = etype;
	ppaid = process_pa_info(context, creds->client, a, &paid,
				rep->padata);
    }
    if (ppaid == NULL)
	ppaid = ctx->ppaid;
    if (ppaid == NULL) {
	ret = krb5_get_pw_salt (context, creds->client, &paid.salt);
	if (ret)
	    return ret;
	paid.etype = etype;
	paid.s2kparams = NULL;
	ppaid = &paid;
    }

    pa = NULL;
    if (rep->padata) {
	int idx = 0;
	pa = krb5_find_padata(rep->padata->val,
			      rep->padata->len,
			      KRB5_PADATA_PK_AS_REP,
			      &idx);
	if (pa == NULL) {
	    idx = 0;
	    pa = krb5_find_padata(rep->padata->val,
				  rep->padata->len,
				  KRB5_PADATA_PK_AS_REP_19,
				  &idx);
	}
    }
    if (pa && ctx->pk_init_ctx) {
#ifdef PKINIT
	_krb5_debug(context, 5, "krb5_get_init_creds: using PKINIT");

	ret = _krb5_pk_rd_pa_reply(context,
				   a->req_body.realm,
				   ctx->pk_init_ctx,
				   etype,
				   hi,
				   ctx->pk_nonce,
				   &ctx->req_buffer,
				   pa,
				   key);
#else
	ret = EINVAL;
	krb5_set_error_message(context, ret, N_("no support for PKINIT compiled in", ""));
#endif
    } else if (ctx->keyseed) {
 	_krb5_debug(context, 5, "krb5_get_init_creds: using keyproc");
	ret = pa_data_to_key_plain(context, creds->client, ctx,
				   ppaid->salt, ppaid->s2kparams, etype, key);
    } else {
	ret = EINVAL;
	krb5_set_error_message(context, ret, N_("No usable pa data type", ""));
    }

    free_paid(context, &paid);
    return ret;
}
