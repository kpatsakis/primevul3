pk_rd_pa_reply_enckey(krb5_context context,
		      int type,
		      const heim_octet_string *indata,
		      const heim_oid *dataType,
		      const char *realm,
		      krb5_pk_init_ctx ctx,
		      krb5_enctype etype,
		      const krb5_krbhst_info *hi,
	       	      unsigned nonce,
		      const krb5_data *req_buffer,
	       	      PA_DATA *pa,
	       	      krb5_keyblock **key)
{
    krb5_error_code ret;
    struct krb5_pk_cert *host = NULL;
    krb5_data content;
    heim_oid contentType = { 0, NULL };
    int flags = HX509_CMS_UE_DONT_REQUIRE_KU_ENCIPHERMENT;

    if (der_heim_oid_cmp(&asn1_oid_id_pkcs7_envelopedData, dataType)) {
	krb5_set_error_message(context, EINVAL,
			       N_("PKINIT: Invalid content type", ""));
	return EINVAL;
    }

    if (ctx->type == PKINIT_WIN2K)
	flags |= HX509_CMS_UE_ALLOW_WEAK;

    ret = hx509_cms_unenvelope(context->hx509ctx,
			       ctx->id->certs,
			       flags,
			       indata->data,
			       indata->length,
			       NULL,
			       0,
			       &contentType,
			       &content);
    if (ret) {
	pk_copy_error(context, context->hx509ctx, ret,
		      "Failed to unenvelope CMS data in PK-INIT reply");
	return ret;
    }
    der_free_oid(&contentType);

    /* win2k uses ContentInfo */
    if (type == PKINIT_WIN2K) {
	heim_oid type2;
	heim_octet_string out;

	ret = hx509_cms_unwrap_ContentInfo(&content, &type2, &out, NULL);
	if (ret) {
	    /* windows LH with interesting CMS packets */
	    size_t ph = 1 + der_length_len(content.length);
	    unsigned char *ptr = malloc(content.length + ph);
	    size_t l;

	    memcpy(ptr + ph, content.data, content.length);

	    ret = der_put_length_and_tag (ptr + ph - 1, ph, content.length,
					  ASN1_C_UNIV, CONS, UT_Sequence, &l);
	    if (ret) {
                free(ptr);
		return ret;
            }
	    free(content.data);
	    content.data = ptr;
	    content.length += ph;

	    ret = hx509_cms_unwrap_ContentInfo(&content, &type2, &out, NULL);
	    if (ret)
		goto out;
	}
	if (der_heim_oid_cmp(&type2, &asn1_oid_id_pkcs7_signedData)) {
	    ret = EINVAL; /* XXX */
	    krb5_set_error_message(context, ret,
				   N_("PKINIT: Invalid content type", ""));
	    der_free_oid(&type2);
	    der_free_octet_string(&out);
	    goto out;
	}
	der_free_oid(&type2);
	krb5_data_free(&content);
	ret = krb5_data_copy(&content, out.data, out.length);
	der_free_octet_string(&out);
	if (ret) {
	    krb5_set_error_message(context, ret,
				   N_("malloc: out of memory", ""));
	    goto out;
	}
    }

    ret = pk_verify_sign(context,
			 content.data,
			 content.length,
			 ctx->id,
			 &contentType,
			 &content,
			 &host);
    if (ret)
	goto out;

    /* make sure that it is the kdc's certificate */
    ret = pk_verify_host(context, realm, hi, ctx, host);
    if (ret) {
	goto out;
    }

#if 0
    if (type == PKINIT_WIN2K) {
	if (der_heim_oid_cmp(&contentType, &asn1_oid_id_pkcs7_data) != 0) {
	    ret = KRB5KRB_AP_ERR_MSG_TYPE;
	    krb5_set_error_message(context, ret, "PKINIT: reply key, wrong oid");
	    goto out;
	}
    } else {
	if (der_heim_oid_cmp(&contentType, &asn1_oid_id_pkrkeydata) != 0) {
	    ret = KRB5KRB_AP_ERR_MSG_TYPE;
	    krb5_set_error_message(context, ret, "PKINIT: reply key, wrong oid");
	    goto out;
	}
    }
#endif

    switch(type) {
    case PKINIT_WIN2K:
	ret = get_reply_key(context, &content, req_buffer, key);
	if (ret != 0 && ctx->require_binding == 0)
	    ret = get_reply_key_win(context, &content, nonce, key);
	break;
    case PKINIT_27:
	ret = get_reply_key(context, &content, req_buffer, key);
	break;
    }
    if (ret)
	goto out;

    /* XXX compare given etype with key->etype */

 out:
    if (host)
	_krb5_pk_cert_free(host);
    der_free_oid(&contentType);
    krb5_data_free(&content);

     return ret;
 }
