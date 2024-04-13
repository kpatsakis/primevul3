krb5_init_creds_init(krb5_context context,
		     krb5_principal client,
		     krb5_prompter_fct prompter,
		     void *prompter_data,
		     krb5_deltat start_time,
		     krb5_get_init_creds_opt *options,
		     krb5_init_creds_context *rctx)
{
    krb5_init_creds_context ctx;
    krb5_error_code ret;

    *rctx = NULL;

    ctx = calloc(1, sizeof(*ctx));
    if (ctx == NULL)
	return krb5_enomem(context);

    ret = get_init_creds_common(context, client, start_time, options, ctx);
    if (ret) {
	free(ctx);
	return ret;
    }

    /* Set a new nonce. */
    krb5_generate_random_block (&ctx->nonce, sizeof(ctx->nonce));
    ctx->nonce &= 0x7fffffff;
    /* XXX these just needs to be the same when using Windows PK-INIT */
    ctx->pk_nonce = ctx->nonce;

    ctx->prompter = prompter;
    ctx->prompter_data = prompter_data;

    *rctx = ctx;

    return ret;
}
