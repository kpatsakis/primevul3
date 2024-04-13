add_enc_ts_padata(krb5_context context,
		  METHOD_DATA *md,
		  krb5_principal client,
		  krb5_s2k_proc keyproc,
		  krb5_const_pointer keyseed,
		  krb5_enctype *enctypes,
		  unsigned netypes,
		  krb5_salt *salt,
		  krb5_data *s2kparams)
{
    krb5_error_code ret;
    krb5_salt salt2;
    krb5_enctype *ep;
    size_t i;

    if(salt == NULL) {
	/* default to standard salt */
	ret = krb5_get_pw_salt (context, client, &salt2);
	if (ret)
	    return ret;
	salt = &salt2;
    }
    if (!enctypes) {
	enctypes = context->etypes;
	netypes = 0;
	for (ep = enctypes; *ep != (krb5_enctype)ETYPE_NULL; ep++)
	    netypes++;
    }

    for (i = 0; i < netypes; ++i) {
	krb5_keyblock *key;

	_krb5_debug(context, 5, "krb5_get_init_creds: using ENC-TS with enctype %d", enctypes[i]);

	ret = (*keyproc)(context, enctypes[i], keyseed,
			 *salt, s2kparams, &key);
	if (ret)
	    continue;
	ret = make_pa_enc_timestamp (context, md, enctypes[i], key);
	krb5_free_keyblock (context, key);
	if (ret)
	    return ret;
    }
    if(salt == &salt2)
	krb5_free_salt(context, salt2);
    return 0;
}
