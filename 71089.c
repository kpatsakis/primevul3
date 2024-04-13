krb5_init_creds_get_creds(krb5_context context,
			  krb5_init_creds_context ctx,
			  krb5_creds *cred)
{
    return krb5_copy_creds_contents(context, &ctx->cred, cred);
}
