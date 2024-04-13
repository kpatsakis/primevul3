krb5_init_creds_free(krb5_context context,
		     krb5_init_creds_context ctx)
{
    free_init_creds_ctx(context, ctx);
    free(ctx);
}
