_krb5_parse_moduli(krb5_context context, const char *file,
		   struct krb5_dh_moduli ***moduli)
{
    /* name bits P G Q */
    krb5_error_code ret;
    struct krb5_dh_moduli **m = NULL, **m2;
    char buf[4096];
    FILE *f;
    int lineno = 0, n = 0;

    *moduli = NULL;

    m = calloc(1, sizeof(m[0]) * 3);
    if (m == NULL)
	return krb5_enomem(context);

    strlcpy(buf, default_moduli_rfc3526_MODP_group14, sizeof(buf));
    ret = _krb5_parse_moduli_line(context, "builtin", 1, buf,  &m[0]);
    if (ret) {
	_krb5_free_moduli(m);
	return ret;
    }
    n++;

    strlcpy(buf, default_moduli_RFC2412_MODP_group2, sizeof(buf));
    ret = _krb5_parse_moduli_line(context, "builtin", 1, buf,  &m[1]);
    if (ret) {
	_krb5_free_moduli(m);
	return ret;
    }
    n++;


    if (file == NULL)
	file = MODULI_FILE;

#ifdef KRB5_USE_PATH_TOKENS
    {
        char * exp_file;

	if (_krb5_expand_path_tokens(context, file, 1, &exp_file) == 0) {
            f = fopen(exp_file, "r");
            krb5_xfree(exp_file);
        } else {
            f = NULL;
        }
    }
#else
    f = fopen(file, "r");
#endif

    if (f == NULL) {
	*moduli = m;
	return 0;
    }
    rk_cloexec_file(f);

    while(fgets(buf, sizeof(buf), f) != NULL) {
	struct krb5_dh_moduli *element;

	buf[strcspn(buf, "\n")] = '\0';
	lineno++;

	m2 = realloc(m, (n + 2) * sizeof(m[0]));
	if (m2 == NULL) {
	    _krb5_free_moduli(m);
	    return krb5_enomem(context);
	}
	m = m2;

	m[n] = NULL;

	ret = _krb5_parse_moduli_line(context, file, lineno, buf,  &element);
	if (ret) {
	    _krb5_free_moduli(m);
	    return ret;
	}
	if (element == NULL)
	    continue;

	m[n] = element;
	m[n + 1] = NULL;
	n++;
    }
    *moduli = m;
    return 0;
}
