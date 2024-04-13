build_auth_pack(krb5_context context,
		unsigned nonce,
		krb5_pk_init_ctx ctx,
		const KDC_REQ_BODY *body,
		AuthPack *a)
{
    size_t buf_size, len = 0;
    krb5_error_code ret;
    void *buf;
    krb5_timestamp sec;
    int32_t usec;
    Checksum checksum;

    krb5_clear_error_message(context);

    memset(&checksum, 0, sizeof(checksum));

    krb5_us_timeofday(context, &sec, &usec);
    a->pkAuthenticator.ctime = sec;
    a->pkAuthenticator.nonce = nonce;

    ASN1_MALLOC_ENCODE(KDC_REQ_BODY, buf, buf_size, body, &len, ret);
    if (ret)
	return ret;
    if (buf_size != len)
	krb5_abortx(context, "internal error in ASN.1 encoder");

    ret = krb5_create_checksum(context,
			       NULL,
			       0,
			       CKSUMTYPE_SHA1,
			       buf,
			       len,
			       &checksum);
    free(buf);
    if (ret)
	return ret;

    ALLOC(a->pkAuthenticator.paChecksum, 1);
    if (a->pkAuthenticator.paChecksum == NULL) {
	return krb5_enomem(context);
    }

    ret = krb5_data_copy(a->pkAuthenticator.paChecksum,
			 checksum.checksum.data, checksum.checksum.length);
    free_Checksum(&checksum);
    if (ret)
	return ret;

    if (ctx->keyex == USE_DH || ctx->keyex == USE_ECDH) {
	const char *moduli_file;
	unsigned long dh_min_bits;
	krb5_data dhbuf;
	size_t size = 0;

	krb5_data_zero(&dhbuf);



	moduli_file = krb5_config_get_string(context, NULL,
					     "libdefaults",
					     "moduli",
					     NULL);

	dh_min_bits =
	    krb5_config_get_int_default(context, NULL, 0,
					"libdefaults",
					"pkinit_dh_min_bits",
					NULL);

	ret = _krb5_parse_moduli(context, moduli_file, &ctx->m);
	if (ret)
	    return ret;

	ctx->u.dh = DH_new();
	if (ctx->u.dh == NULL)
	    return krb5_enomem(context);

	ret = select_dh_group(context, ctx->u.dh, dh_min_bits, ctx->m);
	if (ret)
	    return ret;

	if (DH_generate_key(ctx->u.dh) != 1) {
	    krb5_set_error_message(context, ENOMEM,
				   N_("pkinit: failed to generate DH key", ""));
	    return ENOMEM;
	}


	if (1 /* support_cached_dh */) {
	    ALLOC(a->clientDHNonce, 1);
	    if (a->clientDHNonce == NULL) {
		krb5_clear_error_message(context);
		return ENOMEM;
	    }
	    ret = krb5_data_alloc(a->clientDHNonce, 40);
	    if (a->clientDHNonce == NULL) {
		krb5_clear_error_message(context);
		return ret;
	    }
	    RAND_bytes(a->clientDHNonce->data, a->clientDHNonce->length);
	    ret = krb5_copy_data(context, a->clientDHNonce,
				 &ctx->clientDHNonce);
	    if (ret)
		return ret;
	}

	ALLOC(a->clientPublicValue, 1);
	if (a->clientPublicValue == NULL)
	    return ENOMEM;

	if (ctx->keyex == USE_DH) {
	    DH *dh = ctx->u.dh;
	    DomainParameters dp;
	    heim_integer dh_pub_key;

	    ret = der_copy_oid(&asn1_oid_id_dhpublicnumber,
			       &a->clientPublicValue->algorithm.algorithm);
	    if (ret)
		return ret;

	    memset(&dp, 0, sizeof(dp));

	    ret = BN_to_integer(context, dh->p, &dp.p);
	    if (ret) {
		free_DomainParameters(&dp);
		return ret;
	    }
	    ret = BN_to_integer(context, dh->g, &dp.g);
	    if (ret) {
		free_DomainParameters(&dp);
		return ret;
	    }
	    dp.q = calloc(1, sizeof(*dp.q));
	    if (dp.q == NULL) {
		free_DomainParameters(&dp);
		return ENOMEM;
	    }
	    ret = BN_to_integer(context, dh->q, dp.q);
	    if (ret) {
		free_DomainParameters(&dp);
		return ret;
	    }
	    dp.j = NULL;
	    dp.validationParms = NULL;

	    a->clientPublicValue->algorithm.parameters =
		malloc(sizeof(*a->clientPublicValue->algorithm.parameters));
	    if (a->clientPublicValue->algorithm.parameters == NULL) {
		free_DomainParameters(&dp);
		return ret;
	    }

	    ASN1_MALLOC_ENCODE(DomainParameters,
			       a->clientPublicValue->algorithm.parameters->data,
			       a->clientPublicValue->algorithm.parameters->length,
			       &dp, &size, ret);
	    free_DomainParameters(&dp);
	    if (ret)
		return ret;
	    if (size != a->clientPublicValue->algorithm.parameters->length)
		krb5_abortx(context, "Internal ASN1 encoder error");

	    ret = BN_to_integer(context, dh->pub_key, &dh_pub_key);
	    if (ret)
		return ret;

	    ASN1_MALLOC_ENCODE(DHPublicKey, dhbuf.data, dhbuf.length,
			       &dh_pub_key, &size, ret);
	    der_free_heim_integer(&dh_pub_key);
	    if (ret)
		return ret;
	    if (size != dhbuf.length)
		krb5_abortx(context, "asn1 internal error");
            a->clientPublicValue->subjectPublicKey.length = dhbuf.length * 8;
            a->clientPublicValue->subjectPublicKey.data = dhbuf.data;
	} else if (ctx->keyex == USE_ECDH) {
            ret = _krb5_build_authpack_subjectPK_EC(context, ctx, a);
            if (ret)
                return ret;
	} else
	    krb5_abortx(context, "internal error");
    }

    {
	a->supportedCMSTypes = calloc(1, sizeof(*a->supportedCMSTypes));
	if (a->supportedCMSTypes == NULL)
	    return ENOMEM;

	ret = hx509_crypto_available(context->hx509ctx, HX509_SELECT_ALL,
				     ctx->id->cert,
				     &a->supportedCMSTypes->val,
				     &a->supportedCMSTypes->len);
	if (ret)
	    return ret;
    }

    return ret;
}
