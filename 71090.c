krb5_init_creds_get_error(krb5_context context,
			  krb5_init_creds_context ctx,
			  KRB_ERROR *error)
{
    krb5_error_code ret;

    ret = copy_KRB_ERROR(&ctx->error, error);
    if (ret)
	krb5_enomem(context);

    return ret;
}
