pk_verify_sign(krb5_context context,
	       const void *data,
	       size_t length,
	       struct krb5_pk_identity *id,
	       heim_oid *contentType,
	       krb5_data *content,
	       struct krb5_pk_cert **signer)
{
    hx509_certs signer_certs;
    int ret, flags = 0;

    /* BTMM is broken in Leo and SnowLeo */
    if (id->flags & PKINIT_BTMM) {
	flags |= HX509_CMS_VS_ALLOW_DATA_OID_MISMATCH;
	flags |= HX509_CMS_VS_NO_KU_CHECK;
	flags |= HX509_CMS_VS_NO_VALIDATE;
    }

    *signer = NULL;

    ret = hx509_cms_verify_signed(context->hx509ctx,
				  id->verify_ctx,
				  flags,
				  data,
				  length,
				  NULL,
				  id->certpool,
				  contentType,
				  content,
				  &signer_certs);
    if (ret) {
	pk_copy_error(context, context->hx509ctx, ret,
		      "CMS verify signed failed");
	return ret;
    }

    *signer = calloc(1, sizeof(**signer));
    if (*signer == NULL) {
	krb5_clear_error_message(context);
	ret = ENOMEM;
	goto out;
    }

    ret = hx509_get_one_cert(context->hx509ctx, signer_certs, &(*signer)->cert);
    if (ret) {
	pk_copy_error(context, context->hx509ctx, ret,
		      "Failed to get on of the signer certs");
	goto out;
    }

 out:
    hx509_certs_free(&signer_certs);
    if (ret) {
	if (*signer) {
	    hx509_cert_free((*signer)->cert);
	    free(*signer);
	    *signer = NULL;
	}
    }

    return ret;
}
