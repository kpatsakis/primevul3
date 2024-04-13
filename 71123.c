create_signature(krb5_context context,
		 const heim_oid *eContentType,
		 krb5_data *eContent,
		 struct krb5_pk_identity *id,
		 hx509_peer_info peer,
		 krb5_data *sd_data)
{
    int ret, flags = 0;

    if (id->cert == NULL)
	flags |= HX509_CMS_SIGNATURE_NO_SIGNER;

    ret = hx509_cms_create_signed_1(context->hx509ctx,
				    flags,
				    eContentType,
				    eContent->data,
				    eContent->length,
				    NULL,
				    id->cert,
				    peer,
				    NULL,
				    id->certs,
				    sd_data);
    if (ret) {
	pk_copy_error(context, context->hx509ctx, ret,
		      "Create CMS signedData");
	return ret;
    }

    return 0;
}
