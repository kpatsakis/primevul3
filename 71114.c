_krb5_parse_moduli_line(krb5_context context,
			const char *file,
			int lineno,
			char *p,
			struct krb5_dh_moduli **m)
{
    struct krb5_dh_moduli *m1;
    char *p1;
    int ret;

    *m = NULL;

    m1 = calloc(1, sizeof(*m1));
    if (m1 == NULL)
	return krb5_enomem(context);

    while (isspace((unsigned char)*p))
	p++;
    if (*p  == '#') {
        free(m1);
	return 0;
    }
    ret = EINVAL;

    p1 = strsep(&p, " \t");
    if (p1 == NULL) {
	krb5_set_error_message(context, ret,
			       N_("moduli file %s missing name on line %d", ""),
			       file, lineno);
	goto out;
    }
    m1->name = strdup(p1);
    if (m1->name == NULL) {
	ret = krb5_enomem(context);
	goto out;
    }

    p1 = strsep(&p, " \t");
    if (p1 == NULL) {
	krb5_set_error_message(context, ret,
			       N_("moduli file %s missing bits on line %d", ""),
			       file, lineno);
	goto out;
    }

    m1->bits = atoi(p1);
    if (m1->bits == 0) {
	krb5_set_error_message(context, ret,
			       N_("moduli file %s have un-parsable "
				  "bits on line %d", ""), file, lineno);
	goto out;
    }

    ret = parse_integer(context, &p, file, lineno, "p", &m1->p);
    if (ret)
	goto out;
    ret = parse_integer(context, &p, file, lineno, "g", &m1->g);
    if (ret)
	goto out;
    ret = parse_integer(context, &p, file, lineno, "q", &m1->q);
    if (ret)
	goto out;

    *m = m1;

    return 0;
 out:
    free(m1->name);
    der_free_heim_integer(&m1->p);
    der_free_heim_integer(&m1->g);
    der_free_heim_integer(&m1->q);
    free(m1);
    return ret;
}
