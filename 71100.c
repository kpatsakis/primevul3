pa_data_to_md_pkinit(krb5_context context,
		     const AS_REQ *a,
		     const krb5_principal client,
		     int win2k,
		     krb5_get_init_creds_ctx *ctx,
		     METHOD_DATA *md)
{
    if (ctx->pk_init_ctx == NULL)
	return 0;
#ifdef PKINIT
    return _krb5_pk_mk_padata(context,
			      ctx->pk_init_ctx,
			      ctx->ic_flags,
			      win2k,
			      &a->req_body,
			      ctx->pk_nonce,
			      md);
#else
    krb5_set_error_message(context, EINVAL,
			   N_("no support for PKINIT compiled in", ""));
    return EINVAL;
#endif
}
