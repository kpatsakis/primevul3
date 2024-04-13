krb5_init_creds_set_password(krb5_context context,
			     krb5_init_creds_context ctx,
			     const char *password)
{
    if (ctx->password) {
	memset(ctx->password, 0, strlen(ctx->password));
	free(ctx->password);
    }
    if (password) {
	ctx->password = strdup(password);
	if (ctx->password == NULL)
	    return krb5_enomem(context);
	ctx->keyseed = (void *) ctx->password;
    } else {
	ctx->keyseed = NULL;
	ctx->password = NULL;
    }

    return 0;
}
