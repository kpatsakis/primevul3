_krb5_free_moduli(struct krb5_dh_moduli **moduli)
{
    int i;
    for (i = 0; moduli[i] != NULL; i++) {
	free(moduli[i]->name);
	der_free_heim_integer(&moduli[i]->p);
	der_free_heim_integer(&moduli[i]->g);
	der_free_heim_integer(&moduli[i]->q);
	free(moduli[i]);
    }
    free(moduli);
}
