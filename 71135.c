pk_copy_error(krb5_context context,
	      hx509_context hx509ctx,
	      int hxret,
	      const char *fmt,
	      ...)
{
    va_list va;
    char *s, *f;
    int ret;

    va_start(va, fmt);
    ret = vasprintf(&f, fmt, va);
    va_end(va);
    if (ret == -1 || f == NULL) {
	krb5_clear_error_message(context);
	return;
    }

    s = hx509_get_error_string(hx509ctx, hxret);
    if (s == NULL) {
	krb5_clear_error_message(context);
	free(f);
	return;
    }
    krb5_set_error_message(context, hxret, "%s: %s", f, s);
    free(s);
    free(f);
}
