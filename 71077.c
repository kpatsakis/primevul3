fast_wrap_req(krb5_context context, struct fast_state *state, KDC_REQ *req)
{
    KrbFastArmor *fxarmor = NULL;
    PA_FX_FAST_REQUEST fxreq;
    krb5_error_code ret;
    KrbFastReq fastreq;
    krb5_data data;
    size_t size;

    if (state->flags & KRB5_FAST_DISABLED) {
	_krb5_debug(context, 10, "fast disabled, not doing any fast wrapping");
	return 0;
    }

    memset(&fxreq, 0, sizeof(fxreq));
    memset(&fastreq, 0, sizeof(fastreq));
    krb5_data_zero(&data);

    if (state->armor_crypto == NULL) {
	if (state->armor_ccache) {
	    /*
	     * Instead of keeping state in FX_COOKIE in the KDC, we
	     * rebuild a new armor key for every request, because this
	     * is what the MIT KDC expect and RFC6113 is vage about
	     * what the behavior should be.
	     */
	    state->type = choice_PA_FX_FAST_REQUEST_armored_data;
	} else {
	    return check_fast(context, state);
	}
    }

    state->flags |= KRB5_FAST_EXPECTED;

    fastreq.fast_options.hide_client_names = 1;

    ret = copy_KDC_REQ_BODY(&req->req_body, &fastreq.req_body);
    free_KDC_REQ_BODY(&req->req_body);

    req->req_body.realm = strdup(KRB5_ANON_REALM);
    if ((ALLOC(req->req_body.cname, 1)) != NULL) {
        req->req_body.cname->name_type = KRB5_NT_WELLKNOWN;
    if ((ALLOC(req->req_body.cname->name_string.val, 2)) != NULL) {
        req->req_body.cname->name_string.len = 2;
        req->req_body.cname->name_string.val[0] = strdup(KRB5_WELLKNOWN_NAME);
        req->req_body.cname->name_string.val[1] = strdup(KRB5_ANON_NAME);
        if (req->req_body.cname->name_string.val[0] == NULL ||
            req->req_body.cname->name_string.val[1] == NULL)
            ret = krb5_enomem(context);
      } else
          ret = krb5_enomem(context);
    } else
        ret = krb5_enomem(context);
    if ((ALLOC(req->req_body.till, 1)) != NULL)
        *req->req_body.till = 0;
    else
        ret = krb5_enomem(context);
    if (ret)
        goto out;

    if (req->padata) {
	ret = copy_METHOD_DATA(req->padata, &fastreq.padata);
	free_METHOD_DATA(req->padata);
    } else {
	if ((ALLOC(req->padata, 1)) == NULL)
            ret = krb5_enomem(context);
    }
    if (ret)
        goto out;

    ASN1_MALLOC_ENCODE(KrbFastReq, data.data, data.length, &fastreq, &size, ret);
    if (ret)
	goto out;
    heim_assert(data.length == size, "ASN.1 internal error");

    fxreq.element = state->type;

    if (state->type == choice_PA_FX_FAST_REQUEST_armored_data) {
	size_t len;
	void *buf;

	ret = make_fast_ap_fxarmor(context, state, fastreq.req_body.realm, &fxreq.u.armored_data.armor);
	if (ret)
	    goto out;

	heim_assert(state->armor_crypto != NULL, "FAST armor key missing when FAST started");

	ASN1_MALLOC_ENCODE(KDC_REQ_BODY, buf, len, &req->req_body, &size, ret);
	if (ret)
	    goto out;
	heim_assert(len == size, "ASN.1 internal error");

	ret = krb5_create_checksum(context, state->armor_crypto,
				   KRB5_KU_FAST_REQ_CHKSUM, 0,
				   buf, len, 
				   &fxreq.u.armored_data.req_checksum);
	free(buf);
	if (ret)
	    goto out;

	ret = krb5_encrypt_EncryptedData(context, state->armor_crypto,
					 KRB5_KU_FAST_ENC,
					 data.data,
					 data.length,
					 0,
					 &fxreq.u.armored_data.enc_fast_req);
	krb5_data_free(&data);
        if (ret)
            goto out;

    } else {
	krb5_data_free(&data);
	heim_assert(false, "unknown FAST type, internal error");
    }

    ASN1_MALLOC_ENCODE(PA_FX_FAST_REQUEST, data.data, data.length, &fxreq, &size, ret);
    if (ret)
	goto out;
    heim_assert(data.length == size, "ASN.1 internal error");


    ret = krb5_padata_add(context, req->padata, KRB5_PADATA_FX_FAST, data.data, data.length);
    if (ret)
	goto out;
    krb5_data_zero(&data);

 out:
    free_PA_FX_FAST_REQUEST(&fxreq);
    free_KrbFastReq(&fastreq);
    if (fxarmor) {
	free_KrbFastArmor(fxarmor);
	free(fxarmor);
    }
    krb5_data_free(&data);

    return ret;
}
