cert2epi(hx509_context context, void *ctx, hx509_cert c)
{
    ExternalPrincipalIdentifiers *ids = ctx;
    ExternalPrincipalIdentifier id;
    hx509_name subject = NULL;
    void *p;
    int ret;

    if (ids->len > 10)
	return 0;

    memset(&id, 0, sizeof(id));

    ret = hx509_cert_get_subject(c, &subject);
    if (ret)
	return ret;

    if (hx509_name_is_null_p(subject) != 0) {

	id.subjectName = calloc(1, sizeof(*id.subjectName));
	if (id.subjectName == NULL) {
	    hx509_name_free(&subject);
	    free_ExternalPrincipalIdentifier(&id);
	    return ENOMEM;
	}

	ret = hx509_name_binary(subject, id.subjectName);
	if (ret) {
	    hx509_name_free(&subject);
	    free_ExternalPrincipalIdentifier(&id);
	    return ret;
	}
    }
    hx509_name_free(&subject);


    id.issuerAndSerialNumber = calloc(1, sizeof(*id.issuerAndSerialNumber));
    if (id.issuerAndSerialNumber == NULL) {
	free_ExternalPrincipalIdentifier(&id);
	return ENOMEM;
    }

    {
	IssuerAndSerialNumber iasn;
	hx509_name issuer;
	size_t size = 0;

	memset(&iasn, 0, sizeof(iasn));

	ret = hx509_cert_get_issuer(c, &issuer);
	if (ret) {
	    free_ExternalPrincipalIdentifier(&id);
	    return ret;
	}

	ret = hx509_name_to_Name(issuer, &iasn.issuer);
	hx509_name_free(&issuer);
	if (ret) {
	    free_ExternalPrincipalIdentifier(&id);
	    return ret;
	}

	ret = hx509_cert_get_serialnumber(c, &iasn.serialNumber);
	if (ret) {
	    free_IssuerAndSerialNumber(&iasn);
	    free_ExternalPrincipalIdentifier(&id);
	    return ret;
	}

	ASN1_MALLOC_ENCODE(IssuerAndSerialNumber,
			   id.issuerAndSerialNumber->data,
			   id.issuerAndSerialNumber->length,
			   &iasn, &size, ret);
	free_IssuerAndSerialNumber(&iasn);
	if (ret) {
            free_ExternalPrincipalIdentifier(&id);
	    return ret;
        }
	if (id.issuerAndSerialNumber->length != size)
	    abort();
    }

    id.subjectKeyIdentifier = NULL;

    p = realloc(ids->val, sizeof(ids->val[0]) * (ids->len + 1));
    if (p == NULL) {
	free_ExternalPrincipalIdentifier(&id);
	return ENOMEM;
    }

    ids->val = p;
    ids->val[ids->len] = id;
    ids->len++;

    return 0;
}
