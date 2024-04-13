krb5_init_creds_set_keytab(krb5_context context,
			   krb5_init_creds_context ctx,
			   krb5_keytab keytab)
{
    krb5_keytab_key_proc_args *a;
    krb5_keytab_entry entry;
    krb5_kt_cursor cursor;
    krb5_enctype *etypes = NULL;
    krb5_error_code ret;
    size_t netypes = 0;
    int kvno = 0, found = 0;

    a = malloc(sizeof(*a));
    if (a == NULL)
	return krb5_enomem(context);

    a->principal = ctx->cred.client;
    a->keytab    = keytab;

    ctx->keytab_data = a;
    ctx->keyseed = (void *)a;
    ctx->keyproc = keytab_key_proc;

    /*
     * We need to the KDC what enctypes we support for this keytab,
     * esp if the keytab is really a password based entry, then the
     * KDC might have more enctypes in the database then what we have
     * in the keytab.
     */

    ret = krb5_kt_start_seq_get(context, keytab, &cursor);
    if(ret)
	goto out;

    while(krb5_kt_next_entry(context, keytab, &entry, &cursor) == 0){
	void *ptr;

	if (!krb5_principal_compare(context, entry.principal, ctx->cred.client))
	    goto next;

	found = 1;

	/* check if we ahve this kvno already */
	if (entry.vno > kvno) {
	    /* remove old list of etype */
	    if (etypes)
		free(etypes);
	    etypes = NULL;
	    netypes = 0;
	    kvno = entry.vno;
	} else if (entry.vno != kvno)
	    goto next;

	/* check if enctype is supported */
	if (krb5_enctype_valid(context, entry.keyblock.keytype) != 0)
	    goto next;

	/* add enctype to supported list */
	ptr = realloc(etypes, sizeof(etypes[0]) * (netypes + 2));
	if (ptr == NULL) {
	    free(etypes);
	    ret = krb5_enomem(context);
	    goto out;
	}

	etypes = ptr;
	etypes[netypes] = entry.keyblock.keytype;
	etypes[netypes + 1] = ETYPE_NULL;
	netypes++;
    next:
	krb5_kt_free_entry(context, &entry);
    }
    krb5_kt_end_seq_get(context, keytab, &cursor);

    if (etypes) {
	if (ctx->etypes)
	    free(ctx->etypes);
	ctx->etypes = etypes;
    }

 out:
    if (!found) {
	if (ret == 0)
	    ret = KRB5_KT_NOTFOUND;
	_krb5_kt_principal_not_found(context, ret, keytab, ctx->cred.client, 0, 0);
    }

    return ret;
}
