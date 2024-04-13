_krb5_pk_rd_pa_reply(krb5_context context,
		     const char *realm,
		     void *c,
		     krb5_enctype etype,
		     const krb5_krbhst_info *hi,
		     unsigned nonce,
		     const krb5_data *req_buffer,
		     PA_DATA *pa,
		     krb5_keyblock **key)
{
    krb5_pk_init_ctx ctx = c;
    krb5_error_code ret;
    size_t size;

    /* Check for IETF PK-INIT first */
    if (ctx->type == PKINIT_27) {
	PA_PK_AS_REP rep;
	heim_octet_string os, data;
	heim_oid oid;

	if (pa->padata_type != KRB5_PADATA_PK_AS_REP) {
	    krb5_set_error_message(context, EINVAL,
				   N_("PKINIT: wrong padata recv", ""));
	    return EINVAL;
	}

	ret = decode_PA_PK_AS_REP(pa->padata_value.data,
				  pa->padata_value.length,
				  &rep,
				  &size);
	if (ret) {
	    krb5_set_error_message(context, ret,
				   N_("Failed to decode pkinit AS rep", ""));
	    return ret;
	}

	switch (rep.element) {
	case choice_PA_PK_AS_REP_dhInfo:
	    _krb5_debug(context, 5, "krb5_get_init_creds: using pkinit dh");
	    os = rep.u.dhInfo.dhSignedData;
	    break;
	case choice_PA_PK_AS_REP_encKeyPack:
	    _krb5_debug(context, 5, "krb5_get_init_creds: using kinit enc reply key");
	    os = rep.u.encKeyPack;
	    break;
	default: {
	    PA_PK_AS_REP_BTMM btmm;
	    free_PA_PK_AS_REP(&rep);
	    memset(&rep, 0, sizeof(rep));

	    _krb5_debug(context, 5, "krb5_get_init_creds: using BTMM kinit enc reply key");

	    ret = decode_PA_PK_AS_REP_BTMM(pa->padata_value.data,
					   pa->padata_value.length,
					   &btmm,
					   &size);
	    if (ret) {
		krb5_set_error_message(context, EINVAL,
				       N_("PKINIT: -27 reply "
					  "invalid content type", ""));
		return EINVAL;
	    }

	    if (btmm.dhSignedData || btmm.encKeyPack == NULL) {
		free_PA_PK_AS_REP_BTMM(&btmm);
		ret = EINVAL;
		krb5_set_error_message(context, ret,
				       N_("DH mode not supported for BTMM mode", ""));
		return ret;
	    }

	    /*
	     * Transform to IETF style PK-INIT reply so that free works below
	     */

	    rep.element = choice_PA_PK_AS_REP_encKeyPack;
	    rep.u.encKeyPack.data = btmm.encKeyPack->data;
	    rep.u.encKeyPack.length = btmm.encKeyPack->length;
	    btmm.encKeyPack->data = NULL;
	    btmm.encKeyPack->length = 0;
	    free_PA_PK_AS_REP_BTMM(&btmm);
	    os = rep.u.encKeyPack;
	}
	}

	ret = hx509_cms_unwrap_ContentInfo(&os, &oid, &data, NULL);
	if (ret) {
	    free_PA_PK_AS_REP(&rep);
	    krb5_set_error_message(context, ret,
				   N_("PKINIT: failed to unwrap CI", ""));
	    return ret;
	}

	switch (rep.element) {
	case choice_PA_PK_AS_REP_dhInfo:
	    ret = pk_rd_pa_reply_dh(context, &data, &oid, realm, ctx, etype, hi,
				    ctx->clientDHNonce,
				    rep.u.dhInfo.serverDHNonce,
				    nonce, pa, key);
	    break;
	case choice_PA_PK_AS_REP_encKeyPack:
	    ret = pk_rd_pa_reply_enckey(context, PKINIT_27, &data, &oid, realm,
					ctx, etype, hi, nonce, req_buffer, pa, key);
	    break;
	default:
	    krb5_abortx(context, "pk-init as-rep case not possible to happen");
	}
	der_free_octet_string(&data);
	der_free_oid(&oid);
	free_PA_PK_AS_REP(&rep);

    } else if (ctx->type == PKINIT_WIN2K) {
	PA_PK_AS_REP_Win2k w2krep;

	/* Check for Windows encoding of the AS-REP pa data */

#if 0 /* should this be ? */
	if (pa->padata_type != KRB5_PADATA_PK_AS_REP) {
	    krb5_set_error_message(context, EINVAL,
				   "PKINIT: wrong padata recv");
	    return EINVAL;
	}
#endif

	memset(&w2krep, 0, sizeof(w2krep));

	ret = decode_PA_PK_AS_REP_Win2k(pa->padata_value.data,
					pa->padata_value.length,
					&w2krep,
					&size);
	if (ret) {
	    krb5_set_error_message(context, ret,
				   N_("PKINIT: Failed decoding windows "
				      "pkinit reply %d", ""), (int)ret);
	    return ret;
	}

	krb5_clear_error_message(context);

	switch (w2krep.element) {
	case choice_PA_PK_AS_REP_Win2k_encKeyPack: {
	    heim_octet_string data;
	    heim_oid oid;

	    ret = hx509_cms_unwrap_ContentInfo(&w2krep.u.encKeyPack,
					       &oid, &data, NULL);
	    free_PA_PK_AS_REP_Win2k(&w2krep);
	    if (ret) {
		krb5_set_error_message(context, ret,
				       N_("PKINIT: failed to unwrap CI", ""));
		return ret;
	    }

	    ret = pk_rd_pa_reply_enckey(context, PKINIT_WIN2K, &data, &oid, realm,
					ctx, etype, hi, nonce, req_buffer, pa, key);
	    der_free_octet_string(&data);
	    der_free_oid(&oid);

	    break;
	}
	default:
	    free_PA_PK_AS_REP_Win2k(&w2krep);
	    ret = EINVAL;
	    krb5_set_error_message(context, ret,
				   N_("PKINIT: win2k reply invalid "
				      "content type", ""));
	    break;
	}

    } else {
	ret = EINVAL;
	krb5_set_error_message(context, ret,
			       N_("PKINIT: unknown reply type", ""));
    }

    return ret;
}
