_krb5_pk_mk_padata(krb5_context context,
		   void *c,
		   int ic_flags,
		   int win2k,
		   const KDC_REQ_BODY *req_body,
		   unsigned nonce,
		   METHOD_DATA *md)
{
    krb5_pk_init_ctx ctx = c;
    int win2k_compat;

    if (ctx->id->certs == NULL && ctx->anonymous == 0) {
	krb5_set_error_message(context, HEIM_PKINIT_NO_PRIVATE_KEY,
			       N_("PKINIT: No user certificate given", ""));
	return HEIM_PKINIT_NO_PRIVATE_KEY;
    }

    win2k_compat = krb5_config_get_bool_default(context, NULL,
						win2k,
						"realms",
						req_body->realm,
						"pkinit_win2k",
						NULL);

    if (win2k_compat) {
	ctx->require_binding =
	    krb5_config_get_bool_default(context, NULL,
					 TRUE,
					 "realms",
					 req_body->realm,
					 "pkinit_win2k_require_binding",
					 NULL);
	ctx->type = PKINIT_WIN2K;
    } else
	ctx->type = PKINIT_27;

    ctx->require_eku =
	krb5_config_get_bool_default(context, NULL,
				     TRUE,
				     "realms",
				     req_body->realm,
				     "pkinit_require_eku",
				     NULL);
    if (ic_flags & KRB5_INIT_CREDS_NO_C_NO_EKU_CHECK)
	ctx->require_eku = 0;
    if (ctx->id->flags & PKINIT_BTMM)
	ctx->require_eku = 0;

    ctx->require_krbtgt_otherName =
	krb5_config_get_bool_default(context, NULL,
				     TRUE,
				     "realms",
				     req_body->realm,
				     "pkinit_require_krbtgt_otherName",
				     NULL);

    ctx->require_hostname_match =
	krb5_config_get_bool_default(context, NULL,
				     FALSE,
				     "realms",
				     req_body->realm,
				     "pkinit_require_hostname_match",
				     NULL);

    ctx->trustedCertifiers =
	krb5_config_get_bool_default(context, NULL,
				     TRUE,
				     "realms",
				     req_body->realm,
				     "pkinit_trustedCertifiers",
				     NULL);

    return pk_mk_padata(context, ctx, req_body, nonce, md);
}
