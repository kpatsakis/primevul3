hx_pass_prompter(void *data, const hx509_prompt *prompter)
{
    krb5_error_code ret;
    krb5_prompt prompt;
    krb5_data password_data;
    struct prompter *p = data;

    password_data.data   = prompter->reply.data;
    password_data.length = prompter->reply.length;

    prompt.prompt = prompter->prompt;
    prompt.hidden = hx509_prompt_hidden(prompter->type);
    prompt.reply  = &password_data;

    switch (prompter->type) {
    case HX509_PROMPT_TYPE_INFO:
	prompt.type   = KRB5_PROMPT_TYPE_INFO;
	break;
    case HX509_PROMPT_TYPE_PASSWORD:
    case HX509_PROMPT_TYPE_QUESTION:
    default:
	prompt.type   = KRB5_PROMPT_TYPE_PASSWORD;
	break;
    }

    ret = (*p->prompter)(p->context, p->prompter_data, NULL, NULL, 1, &prompt);
    if (ret) {
	memset (prompter->reply.data, 0, prompter->reply.length);
	return 1;
    }
    return 0;
}
