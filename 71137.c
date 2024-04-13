 pk_rd_pa_reply_dh(krb5_context context,
 		  const heim_octet_string *indata,
		  const heim_oid *dataType,
		  const char *realm,
		  krb5_pk_init_ctx ctx,
		  krb5_enctype etype,
		  const krb5_krbhst_info *hi,
		  const DHNonce *c_n,
		  const DHNonce *k_n,
                  unsigned nonce,
                  PA_DATA *pa,
                  krb5_keyblock **key)
{
    const unsigned char *p;
    unsigned char *dh_gen_key = NULL;
    struct krb5_pk_cert *host = NULL;
    BIGNUM *kdc_dh_pubkey = NULL;
    KDCDHKeyInfo kdc_dh_info;
    heim_oid contentType = { 0, NULL };
    krb5_data content;
    krb5_error_code ret;
    int dh_gen_keylen = 0;
    size_t size;

    krb5_data_zero(&content);
    memset(&kdc_dh_info, 0, sizeof(kdc_dh_info));

    if (der_heim_oid_cmp(&asn1_oid_id_pkcs7_signedData, dataType)) {
	krb5_set_error_message(context, EINVAL,
			       N_("PKINIT: Invalid content type", ""));
	return EINVAL;
    }

    ret = pk_verify_sign(context,
			 indata->data,
			 indata->length,
			 ctx->id,
			 &contentType,
			 &content,
			 &host);
    if (ret)
	goto out;

    /* make sure that it is the kdc's certificate */
    ret = pk_verify_host(context, realm, hi, ctx, host);
    if (ret)
	goto out;

    if (der_heim_oid_cmp(&contentType, &asn1_oid_id_pkdhkeydata)) {
	ret = KRB5KRB_AP_ERR_MSG_TYPE;
	krb5_set_error_message(context, ret,
			       N_("pkinit - dh reply contains wrong oid", ""));
	goto out;
    }

    ret = decode_KDCDHKeyInfo(content.data,
			      content.length,
			      &kdc_dh_info,
			      &size);

    if (ret) {
	krb5_set_error_message(context, ret,
			       N_("pkinit - failed to decode "
				  "KDC DH Key Info", ""));
	goto out;
    }

    if (kdc_dh_info.nonce != nonce) {
	ret = KRB5KRB_AP_ERR_MODIFIED;
	krb5_set_error_message(context, ret,
			       N_("PKINIT: DH nonce is wrong", ""));
	goto out;
    }

    if (kdc_dh_info.dhKeyExpiration) {
	if (k_n == NULL) {
	    ret = KRB5KRB_ERR_GENERIC;
	    krb5_set_error_message(context, ret,
				   N_("pkinit; got key expiration "
				      "without server nonce", ""));
	    goto out;
	}
	if (c_n == NULL) {
	    ret = KRB5KRB_ERR_GENERIC;
	    krb5_set_error_message(context, ret,
				   N_("pkinit; got DH reuse but no "
				      "client nonce", ""));
	    goto out;
	}
    } else {
	if (k_n) {
	    ret = KRB5KRB_ERR_GENERIC;
	    krb5_set_error_message(context, ret,
				   N_("pkinit: got server nonce "
				      "without key expiration", ""));
	    goto out;
	}
	c_n = NULL;
    }


    p = kdc_dh_info.subjectPublicKey.data;
    size = (kdc_dh_info.subjectPublicKey.length + 7) / 8;

    if (ctx->keyex == USE_DH) {
	DHPublicKey k;
	ret = decode_DHPublicKey(p, size, &k, NULL);
	if (ret) {
	    krb5_set_error_message(context, ret,
				   N_("pkinit: can't decode "
				      "without key expiration", ""));
	    goto out;
	}

	kdc_dh_pubkey = integer_to_BN(context, "DHPublicKey", &k);
	free_DHPublicKey(&k);
	if (kdc_dh_pubkey == NULL) {
	    ret = ENOMEM;
	    goto out;
	}


	size = DH_size(ctx->u.dh);

	dh_gen_key = malloc(size);
	if (dh_gen_key == NULL) {
	    ret = krb5_enomem(context);
	    goto out;
	}

	dh_gen_keylen = DH_compute_key(dh_gen_key, kdc_dh_pubkey, ctx->u.dh);
	if (dh_gen_keylen == -1) {
	    ret = KRB5KRB_ERR_GENERIC;
	    dh_gen_keylen = 0;
	    krb5_set_error_message(context, ret,
				   N_("PKINIT: Can't compute Diffie-Hellman key", ""));
	    goto out;
	}
	if (dh_gen_keylen < (int)size) {
	    size -= dh_gen_keylen;
	    memmove(dh_gen_key + size, dh_gen_key, dh_gen_keylen);
	    memset(dh_gen_key, 0, size);
	}

    } else {
        ret = _krb5_pk_rd_pa_reply_ecdh_compute_key(context, ctx, p,
                                                    size, &dh_gen_key,
                                                    &dh_gen_keylen);
        if (ret)
          goto out;
    }

    if (dh_gen_keylen <= 0) {
	ret = EINVAL;
	krb5_set_error_message(context, ret,
			       N_("PKINIT: resulting DH key <= 0", ""));
	dh_gen_keylen = 0;
	goto out;
    }

    *key = malloc (sizeof (**key));
    if (*key == NULL) {
	ret = krb5_enomem(context);
	goto out;
    }

    ret = _krb5_pk_octetstring2key(context,
				   etype,
				   dh_gen_key, dh_gen_keylen,
				   c_n, k_n,
				   *key);
    if (ret) {
	krb5_set_error_message(context, ret,
			       N_("PKINIT: can't create key from DH key", ""));
	free(*key);
	*key = NULL;
	goto out;
    }

 out:
    if (kdc_dh_pubkey)
	BN_free(kdc_dh_pubkey);
    if (dh_gen_key) {
	memset(dh_gen_key, 0, dh_gen_keylen);
	free(dh_gen_key);
    }
    if (host)
	_krb5_pk_cert_free(host);
    if (content.data)
	krb5_data_free(&content);
    der_free_oid(&contentType);
    free_KDCDHKeyInfo(&kdc_dh_info);

    return ret;
}
