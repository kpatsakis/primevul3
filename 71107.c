report_expiration (krb5_context context,
		   krb5_prompter_fct prompter,
		   krb5_data *data,
		   const char *str,
		   time_t now)
{
    char *p = NULL;

    if (asprintf(&p, "%s%s", str, ctime(&now)) < 0 || p == NULL)
	return;
    (*prompter)(context, data, NULL, p, 0, NULL);
    free(p);
}
