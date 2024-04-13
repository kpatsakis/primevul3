krb5_get_init_creds_password(krb5_context context,
			     krb5_creds *creds,
			     krb5_principal client,
			     const char *password,
			     krb5_prompter_fct prompter,
			     void *data,
			     krb5_deltat start_time,
			     const char *in_tkt_service,
			     krb5_get_init_creds_opt *options)
{
    krb5_init_creds_context ctx;
    char buf[BUFSIZ], buf2[BUFSIZ];
    krb5_error_code ret;
    int chpw = 0;

 again:
    ret = krb5_init_creds_init(context, client, prompter, data, start_time, options, &ctx);
    if (ret)
	goto out;

    ret = krb5_init_creds_set_service(context, ctx, in_tkt_service);
    if (ret)
	goto out;

    if (prompter != NULL && ctx->password == NULL && password == NULL) {
	krb5_prompt prompt;
	krb5_data password_data;
	char *p, *q = NULL;
	int aret;

	ret = krb5_unparse_name(context, client, &p);
	if (ret)
	    goto out;

	aret = asprintf(&q, "%s's Password: ", p);
	free (p);
	if (aret == -1 || q == NULL) {
	    ret = krb5_enomem(context);
	    goto out;
	}
	prompt.prompt = q;
	password_data.data   = buf;
	password_data.length = sizeof(buf);
	prompt.hidden = 1;
	prompt.reply  = &password_data;
	prompt.type   = KRB5_PROMPT_TYPE_PASSWORD;

	ret = (*prompter) (context, data, NULL, NULL, 1, &prompt);
	free (q);
	if (ret) {
	    memset (buf, 0, sizeof(buf));
	    ret = KRB5_LIBOS_PWDINTR;
	    krb5_clear_error_message (context);
	    goto out;
	}
	password = password_data.data;
    }

    if (password) {
	ret = krb5_init_creds_set_password(context, ctx, password);
	if (ret)
	    goto out;
    }

    ret = krb5_init_creds_get(context, ctx);

    if (ret == 0)
	krb5_process_last_request(context, options, ctx);


    if (ret == KRB5KDC_ERR_KEY_EXPIRED && chpw == 0) {
	/* try to avoid recursion */
	if (in_tkt_service != NULL && strcmp(in_tkt_service, "kadmin/changepw") == 0)
	   goto out;

	/* don't try to change password where then where none */
	if (prompter == NULL)
	    goto out;

	if ((options->flags & KRB5_GET_INIT_CREDS_OPT_CHANGE_PASSWORD_PROMPT) &&
            !options->change_password_prompt)
		goto out;

	ret = change_password (context,
			       client,
			       ctx->password,
			       buf2,
			       sizeof(buf2),
			       prompter,
			       data,
			       options);
	if (ret)
	    goto out;
	password = buf2;
	chpw = 1;
	krb5_init_creds_free(context, ctx);
	goto again;
    }

 out:
    if (ret == 0)
	krb5_init_creds_get_creds(context, ctx, creds);

    if (ctx)
	krb5_init_creds_free(context, ctx);

    memset(buf, 0, sizeof(buf));
    memset(buf2, 0, sizeof(buf2));
    return ret;
}
