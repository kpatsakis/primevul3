select_dh_group(krb5_context context, DH *dh, unsigned long bits,
		struct krb5_dh_moduli **moduli)
{
    const struct krb5_dh_moduli *m;

    if (bits == 0) {
	m = moduli[1]; /* XXX */
	if (m == NULL)
	    m = moduli[0]; /* XXX */
    } else {
	int i;
	for (i = 0; moduli[i] != NULL; i++) {
	    if (bits < moduli[i]->bits)
		break;
	}
	if (moduli[i] == NULL) {
	    krb5_set_error_message(context, EINVAL,
				   N_("Did not find a DH group parameter "
				      "matching requirement of %lu bits", ""),
				   bits);
	    return EINVAL;
	}
	m = moduli[i];
    }

    dh->p = integer_to_BN(context, "p", &m->p);
    if (dh->p == NULL)
	return ENOMEM;
    dh->g = integer_to_BN(context, "g", &m->g);
    if (dh->g == NULL)
	return ENOMEM;
    dh->q = integer_to_BN(context, "q", &m->q);
    if (dh->q == NULL)
	return ENOMEM;

    return 0;
}
