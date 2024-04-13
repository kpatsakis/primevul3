_krb5_dh_group_ok(krb5_context context, unsigned long bits,
		  heim_integer *p, heim_integer *g, heim_integer *q,
		  struct krb5_dh_moduli **moduli,
		  char **name)
{
    int i;

    if (name)
	*name = NULL;

    for (i = 0; moduli[i] != NULL; i++) {
	if (der_heim_integer_cmp(&moduli[i]->g, g) == 0 &&
	    der_heim_integer_cmp(&moduli[i]->p, p) == 0 &&
	    (q == NULL || der_heim_integer_cmp(&moduli[i]->q, q) == 0))
	    {
		if (bits && bits > moduli[i]->bits) {
		    krb5_set_error_message(context,
					   KRB5_KDC_ERR_DH_KEY_PARAMETERS_NOT_ACCEPTED,
					   N_("PKINIT: DH group parameter %s "
					      "no accepted, not enough bits "
					      "generated", ""),
					   moduli[i]->name);
		    return KRB5_KDC_ERR_DH_KEY_PARAMETERS_NOT_ACCEPTED;
		}
		if (name)
		    *name = strdup(moduli[i]->name);
		return 0;
	    }
    }
    krb5_set_error_message(context,
			   KRB5_KDC_ERR_DH_KEY_PARAMETERS_NOT_ACCEPTED,
			   N_("PKINIT: DH group parameter no ok", ""));
    return KRB5_KDC_ERR_DH_KEY_PARAMETERS_NOT_ACCEPTED;
}
