pk_mk_padata(krb5_context context,
	     krb5_pk_init_ctx ctx,
	     const KDC_REQ_BODY *req_body,
	     unsigned nonce,
	     METHOD_DATA *md)
{
    struct ContentInfo content_info;
    krb5_error_code ret;
    const heim_oid *oid = NULL;
    size_t size = 0;
    krb5_data buf, sd_buf;
    int pa_type = -1;

    krb5_data_zero(&buf);
    krb5_data_zero(&sd_buf);
    memset(&content_info, 0, sizeof(content_info));

    if (ctx->type == PKINIT_WIN2K) {
	AuthPack_Win2k ap;
	krb5_timestamp sec;
	int32_t usec;

	memset(&ap, 0, sizeof(ap));

	/* fill in PKAuthenticator */
	ret = copy_PrincipalName(req_body->sname, &ap.pkAuthenticator.kdcName);
	if (ret) {
	    free_AuthPack_Win2k(&ap);
	    krb5_clear_error_message(context);
	    goto out;
	}
	ret = copy_Realm(&req_body->realm, &ap.pkAuthenticator.kdcRealm);
	if (ret) {
	    free_AuthPack_Win2k(&ap);
	    krb5_clear_error_message(context);
	    goto out;
	}

	krb5_us_timeofday(context, &sec, &usec);
	ap.pkAuthenticator.ctime = sec;
	ap.pkAuthenticator.cusec = usec;
	ap.pkAuthenticator.nonce = nonce;

	ASN1_MALLOC_ENCODE(AuthPack_Win2k, buf.data, buf.length,
			   &ap, &size, ret);
	free_AuthPack_Win2k(&ap);
	if (ret) {
	    krb5_set_error_message(context, ret,
				   N_("Failed encoding AuthPackWin: %d", ""),
				   (int)ret);
	    goto out;
	}
	if (buf.length != size)
	    krb5_abortx(context, "internal ASN1 encoder error");

	oid = &asn1_oid_id_pkcs7_data;
    } else if (ctx->type == PKINIT_27) {
	AuthPack ap;

	memset(&ap, 0, sizeof(ap));

	ret = build_auth_pack(context, nonce, ctx, req_body, &ap);
	if (ret) {
	    free_AuthPack(&ap);
	    goto out;
	}

	ASN1_MALLOC_ENCODE(AuthPack, buf.data, buf.length, &ap, &size, ret);
	free_AuthPack(&ap);
	if (ret) {
	    krb5_set_error_message(context, ret,
				   N_("Failed encoding AuthPack: %d", ""),
				   (int)ret);
	    goto out;
	}
	if (buf.length != size)
	    krb5_abortx(context, "internal ASN1 encoder error");

	oid = &asn1_oid_id_pkauthdata;
    } else
	krb5_abortx(context, "internal pkinit error");

    ret = create_signature(context, oid, &buf, ctx->id,
			   ctx->peer, &sd_buf);
    krb5_data_free(&buf);
    if (ret)
	goto out;

    ret = hx509_cms_wrap_ContentInfo(&asn1_oid_id_pkcs7_signedData, &sd_buf, &buf);
    krb5_data_free(&sd_buf);
    if (ret) {
	krb5_set_error_message(context, ret,
			       N_("ContentInfo wrapping of signedData failed",""));
	goto out;
    }

    if (ctx->type == PKINIT_WIN2K) {
	PA_PK_AS_REQ_Win2k winreq;

	pa_type = KRB5_PADATA_PK_AS_REQ_WIN;

	memset(&winreq, 0, sizeof(winreq));

	winreq.signed_auth_pack = buf;

	ASN1_MALLOC_ENCODE(PA_PK_AS_REQ_Win2k, buf.data, buf.length,
			   &winreq, &size, ret);
	free_PA_PK_AS_REQ_Win2k(&winreq);

    } else if (ctx->type == PKINIT_27) {
	PA_PK_AS_REQ req;

	pa_type = KRB5_PADATA_PK_AS_REQ;

	memset(&req, 0, sizeof(req));
	req.signedAuthPack = buf;

	if (ctx->trustedCertifiers) {

	    req.trustedCertifiers = calloc(1, sizeof(*req.trustedCertifiers));
	    if (req.trustedCertifiers == NULL) {
		ret = krb5_enomem(context);
		free_PA_PK_AS_REQ(&req);
		goto out;
	    }
	    ret = build_edi(context, context->hx509ctx,
			    ctx->id->anchors, req.trustedCertifiers);
	    if (ret) {
		krb5_set_error_message(context, ret,
				       N_("pk-init: failed to build "
					  "trustedCertifiers", ""));
		free_PA_PK_AS_REQ(&req);
		goto out;
	    }
	}
	req.kdcPkId = NULL;

	ASN1_MALLOC_ENCODE(PA_PK_AS_REQ, buf.data, buf.length,
			   &req, &size, ret);

	free_PA_PK_AS_REQ(&req);

    } else
	krb5_abortx(context, "internal pkinit error");
    if (ret) {
	krb5_set_error_message(context, ret, "PA-PK-AS-REQ %d", (int)ret);
	goto out;
    }
    if (buf.length != size)
	krb5_abortx(context, "Internal ASN1 encoder error");

    ret = krb5_padata_add(context, md, pa_type, buf.data, buf.length);
    if (ret)
	free(buf.data);

    if (ret == 0)
    	krb5_padata_add(context, md, KRB5_PADATA_PK_AS_09_BINDING, NULL, 0);

 out:
    free_ContentInfo(&content_info);

    return ret;
}
