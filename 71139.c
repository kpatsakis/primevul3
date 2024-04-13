pk_verify_host(krb5_context context,
	       const char *realm,
	       const krb5_krbhst_info *hi,
	       struct krb5_pk_init_ctx_data *ctx,
	       struct krb5_pk_cert *host)
{
    krb5_error_code ret = 0;

    if (ctx->require_eku) {
	ret = hx509_cert_check_eku(context->hx509ctx, host->cert,
				   &asn1_oid_id_pkkdcekuoid, 0);
	if (ret) {
	    krb5_set_error_message(context, ret,
				   N_("No PK-INIT KDC EKU in kdc certificate", ""));
	    return ret;
	}
    }
    if (ctx->require_krbtgt_otherName) {
	hx509_octet_string_list list;
	size_t i;
	int matched = 0;

	ret = hx509_cert_find_subjectAltName_otherName(context->hx509ctx,
						       host->cert,
						       &asn1_oid_id_pkinit_san,
						       &list);
	if (ret) {
	    krb5_set_error_message(context, ret,
				   N_("Failed to find the PK-INIT "
				      "subjectAltName in the KDC "
				      "certificate", ""));

	    return ret;
	}

	/*
	 * subjectAltNames are multi-valued, and a single KDC may serve
	 * multiple realms. The SAN validation here must accept
	 * the KDC's cert if *any* of the SANs match the expected KDC.
	 * It is OK for *some* of the SANs to not match, provided at least
	 * one does.
	 */
	for (i = 0; matched == 0 && i < list.len; i++) {
	    KRB5PrincipalName r;

	    ret = decode_KRB5PrincipalName(list.val[i].data,
					   list.val[i].length,
					   &r,
					   NULL);
	    if (ret) {
		krb5_set_error_message(context, ret,
				       N_("Failed to decode the PK-INIT "
					  "subjectAltName in the "
					  "KDC certificate", ""));

		break;
	    }

	    if (r.principalName.name_string.len == 2 &&
		strcmp(r.principalName.name_string.val[0], KRB5_TGS_NAME) == 0
		&& strcmp(r.principalName.name_string.val[1], realm) == 0
		&& strcmp(r.realm, realm) == 0)
		matched = 1;

	    free_KRB5PrincipalName(&r);
	}
	hx509_free_octet_string_list(&list);
	if (matched == 0) {
	    ret = KRB5_KDC_ERR_INVALID_CERTIFICATE;
	    /* XXX: Lost in translation... */
	    krb5_set_error_message(context, ret,
				   N_("KDC have wrong realm name in "
				      "the certificate", ""));
	}
    }
    if (ret)
	return ret;

    if (hi) {
	ret = hx509_verify_hostname(context->hx509ctx, host->cert,
				    ctx->require_hostname_match,
				    HX509_HN_HOSTNAME,
				    hi->hostname,
				    hi->ai->ai_addr, hi->ai->ai_addrlen);

	if (ret)
	    krb5_set_error_message(context, ret,
				   N_("Address mismatch in "
				      "the KDC certificate", ""));
    }
    return ret;
}
