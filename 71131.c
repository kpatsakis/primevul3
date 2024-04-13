krb5_get_init_creds_opt_set_pkinit(krb5_context context,
				   krb5_get_init_creds_opt *opt,
				   krb5_principal principal,
				   const char *user_id,
				   const char *x509_anchors,
				   char * const * pool,
				   char * const * pki_revoke,
				   int flags,
				   krb5_prompter_fct prompter,
				   void *prompter_data,
				   char *password)
{
#ifdef PKINIT
    krb5_error_code ret;
    char *anchors = NULL;

    if (opt->opt_private == NULL) {
	krb5_set_error_message(context, EINVAL,
			       N_("PKINIT: on non extendable opt", ""));
	return EINVAL;
    }

    opt->opt_private->pk_init_ctx =
	calloc(1, sizeof(*opt->opt_private->pk_init_ctx));
    if (opt->opt_private->pk_init_ctx == NULL)
	return krb5_enomem(context);
    opt->opt_private->pk_init_ctx->require_binding = 0;
    opt->opt_private->pk_init_ctx->require_eku = 1;
    opt->opt_private->pk_init_ctx->require_krbtgt_otherName = 1;
    opt->opt_private->pk_init_ctx->peer = NULL;

    /* XXX implement krb5_appdefault_strings  */
    if (pool == NULL)
	pool = krb5_config_get_strings(context, NULL,
				       "appdefaults",
				       "pkinit_pool",
				       NULL);

    if (pki_revoke == NULL)
	pki_revoke = krb5_config_get_strings(context, NULL,
					     "appdefaults",
					     "pkinit_revoke",
					     NULL);

    if (x509_anchors == NULL) {
	krb5_appdefault_string(context, "kinit",
			       krb5_principal_get_realm(context, principal),
			       "pkinit_anchors", NULL, &anchors);
	x509_anchors = anchors;
    }

    if (flags & KRB5_GIC_OPT_PKINIT_ANONYMOUS)
	opt->opt_private->pk_init_ctx->anonymous = 1;

    ret = _krb5_pk_load_id(context,
			   &opt->opt_private->pk_init_ctx->id,
			   user_id,
			   x509_anchors,
			   pool,
			   pki_revoke,
			   prompter,
			   prompter_data,
			   password);
    if (ret) {
	free(opt->opt_private->pk_init_ctx);
	opt->opt_private->pk_init_ctx = NULL;
	return ret;
    }

    if (opt->opt_private->pk_init_ctx->id->certs) {
	_krb5_pk_set_user_id(context,
			     principal,
			     opt->opt_private->pk_init_ctx,
			     opt->opt_private->pk_init_ctx->id->certs);
    } else
	opt->opt_private->pk_init_ctx->id->cert = NULL;

    if ((flags & KRB5_GIC_OPT_PKINIT_USE_ENCKEY) == 0) {
	hx509_context hx509ctx = context->hx509ctx;
	hx509_cert cert = opt->opt_private->pk_init_ctx->id->cert;

	opt->opt_private->pk_init_ctx->keyex = USE_DH;

	/*
	 * If its a ECDSA certs, lets select ECDSA as the keyex algorithm.
	 */
	if (cert) {
	    AlgorithmIdentifier alg;

	    ret = hx509_cert_get_SPKI_AlgorithmIdentifier(hx509ctx, cert, &alg);
	    if (ret == 0) {
		if (der_heim_oid_cmp(&alg.algorithm, &asn1_oid_id_ecPublicKey) == 0)
		    opt->opt_private->pk_init_ctx->keyex = USE_ECDH;
		free_AlgorithmIdentifier(&alg);
	    }
	}

    } else {
	opt->opt_private->pk_init_ctx->keyex = USE_RSA;

	if (opt->opt_private->pk_init_ctx->id->certs == NULL) {
	    krb5_set_error_message(context, EINVAL,
				   N_("No anonymous pkinit support in RSA mode", ""));
	    return EINVAL;
	}
    }

    return 0;
#else
    krb5_set_error_message(context, EINVAL,
			   N_("no support for PKINIT compiled in", ""));
    return EINVAL;
#endif
}
