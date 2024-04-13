free_init_creds_ctx(krb5_context context, krb5_init_creds_context ctx)
{
    if (ctx->etypes)
	free(ctx->etypes);
    if (ctx->pre_auth_types)
	free (ctx->pre_auth_types);
    if (ctx->in_tkt_service)
	free(ctx->in_tkt_service);
    if (ctx->keytab_data)
	free(ctx->keytab_data);
    if (ctx->password) {
	memset(ctx->password, 0, strlen(ctx->password));
	free(ctx->password);
    }
    /*
     * FAST state (we don't close the armor_ccache because we might have
     * to destroy it, and how would we know? also, the caller should
     * take care of cleaning up the armor_ccache).
     */
    if (ctx->fast_state.armor_service)
	krb5_free_principal(context, ctx->fast_state.armor_service);
    if (ctx->fast_state.armor_crypto)
	krb5_crypto_destroy(context, ctx->fast_state.armor_crypto);
    if (ctx->fast_state.strengthen_key)
	krb5_free_keyblock(context, ctx->fast_state.strengthen_key);
    krb5_free_keyblock_contents(context, &ctx->fast_state.armor_key);

    krb5_data_free(&ctx->req_buffer);
    krb5_free_cred_contents(context, &ctx->cred);
    free_METHOD_DATA(&ctx->md);
    free_AS_REP(&ctx->as_rep);
    free_EncKDCRepPart(&ctx->enc_part);
    free_KRB_ERROR(&ctx->error);
    free_AS_REQ(&ctx->as_req);
    if (ctx->ppaid) {
	free_paid(context, ctx->ppaid);
	free(ctx->ppaid);
    }
    memset(ctx, 0, sizeof(*ctx));
}
