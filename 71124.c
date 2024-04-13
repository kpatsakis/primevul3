find_cert(krb5_context context, struct krb5_pk_identity *id,
	  hx509_query *q, hx509_cert *cert)
{
    struct certfind cf[4] = {
	{ "MobileMe EKU", NULL },
	{ "PKINIT EKU", NULL },
	{ "MS EKU", NULL },
	{ "any (or no)", NULL }
    };
    int ret = HX509_CERT_NOT_FOUND;
    size_t i, start = 1;
    unsigned oids[] = { 1, 2, 840, 113635, 100, 3, 2, 1 };
    const heim_oid mobileMe = { sizeof(oids)/sizeof(oids[0]), oids };


    if (id->flags & PKINIT_BTMM)
	start = 0;

    cf[0].oid = &mobileMe;
    cf[1].oid = &asn1_oid_id_pkekuoid;
    cf[2].oid = &asn1_oid_id_pkinit_ms_eku;
    cf[3].oid = NULL;

    for (i = start; i < sizeof(cf)/sizeof(cf[0]); i++) {
	ret = hx509_query_match_eku(q, cf[i].oid);
	if (ret) {
	    pk_copy_error(context, context->hx509ctx, ret,
			  "Failed setting %s OID", cf[i].type);
	    return ret;
	}

	ret = hx509_certs_find(context->hx509ctx, id->certs, q, cert);
	if (ret == 0)
	    break;
	pk_copy_error(context, context->hx509ctx, ret,
		      "Failed finding certificate with %s OID", cf[i].type);
    }
    return ret;
}
