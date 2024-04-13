krb5_pk_enterprise_cert(krb5_context context,
			const char *user_id,
			krb5_const_realm realm,
			krb5_principal *principal,
			struct hx509_certs_data **res)
{
#ifdef PKINIT
    krb5_error_code ret;
    hx509_certs certs, result;
    hx509_cert cert = NULL;
    hx509_query *q;
    char *name;

    *principal = NULL;
    if (res)
	*res = NULL;

    if (user_id == NULL) {
	krb5_set_error_message(context, ENOENT, "no user id");
	return ENOENT;
    }

    ret = hx509_certs_init(context->hx509ctx, user_id, 0, NULL, &certs);
    if (ret) {
	pk_copy_error(context, context->hx509ctx, ret,
		      "Failed to init cert certs");
	goto out;
    }

    ret = hx509_query_alloc(context->hx509ctx, &q);
    if (ret) {
	krb5_set_error_message(context, ret, "out of memory");
	hx509_certs_free(&certs);
	goto out;
    }

    hx509_query_match_option(q, HX509_QUERY_OPTION_PRIVATE_KEY);
    hx509_query_match_option(q, HX509_QUERY_OPTION_KU_DIGITALSIGNATURE);
    hx509_query_match_eku(q, &asn1_oid_id_pkinit_ms_eku);
    hx509_query_match_cmp_func(q, find_ms_san, NULL);

    ret = hx509_certs_filter(context->hx509ctx, certs, q, &result);
    hx509_query_free(context->hx509ctx, q);
    hx509_certs_free(&certs);
    if (ret) {
	pk_copy_error(context, context->hx509ctx, ret,
		      "Failed to find PKINIT certificate");
	return ret;
    }

    ret = hx509_get_one_cert(context->hx509ctx, result, &cert);
    hx509_certs_free(&result);
    if (ret) {
	pk_copy_error(context, context->hx509ctx, ret,
		      "Failed to get one cert");
	goto out;
    }

    ret = get_ms_san(context->hx509ctx, cert, &name);
    if (ret) {
	pk_copy_error(context, context->hx509ctx, ret,
		      "Failed to get MS SAN");
	goto out;
    }

    ret = krb5_make_principal(context, principal, realm, name, NULL);
    free(name);
    if (ret)
	goto out;

    krb5_principal_set_type(context, *principal, KRB5_NT_ENTERPRISE_PRINCIPAL);

    if (res) {
	ret = hx509_certs_init(context->hx509ctx, "MEMORY:", 0, NULL, res);
	if (ret)
	    goto out;

	ret = hx509_certs_add(context->hx509ctx, *res, cert);
	if (ret) {
	    hx509_certs_free(res);
	    goto out;
	}
    }

 out:
    hx509_cert_free(cert);

    return ret;
#else
    krb5_set_error_message(context, EINVAL,
			   N_("no support for PKINIT compiled in", ""));
    return EINVAL;
#endif
}
