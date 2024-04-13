_krb5_get_init_creds_opt_free_pkinit(krb5_get_init_creds_opt *opt)
{
#ifdef PKINIT
    krb5_pk_init_ctx ctx;

    if (opt->opt_private == NULL || opt->opt_private->pk_init_ctx == NULL)
	return;
    ctx = opt->opt_private->pk_init_ctx;
    switch (ctx->keyex) {
    case USE_DH:
	if (ctx->u.dh)
	    DH_free(ctx->u.dh);
	break;
    case USE_RSA:
	break;
    case USE_ECDH:
	if (ctx->u.eckey)
            _krb5_pk_eckey_free(ctx->u.eckey);
	break;
    }
    if (ctx->id) {
	hx509_verify_destroy_ctx(ctx->id->verify_ctx);
	hx509_certs_free(&ctx->id->certs);
	hx509_cert_free(ctx->id->cert);
	hx509_certs_free(&ctx->id->anchors);
	hx509_certs_free(&ctx->id->certpool);

	if (ctx->clientDHNonce) {
	    krb5_free_data(NULL, ctx->clientDHNonce);
	    ctx->clientDHNonce = NULL;
	}
	if (ctx->m)
	    _krb5_free_moduli(ctx->m);
	free(ctx->id);
	ctx->id = NULL;
    }
    free(opt->opt_private->pk_init_ctx);
    opt->opt_private->pk_init_ctx = NULL;
#endif
}
