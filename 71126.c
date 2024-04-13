get_ms_san(hx509_context context, hx509_cert cert, char **upn)
{
    hx509_octet_string_list list;
    int ret;

    *upn = NULL;

    ret = hx509_cert_find_subjectAltName_otherName(context,
						   cert,
						   &asn1_oid_id_pkinit_ms_san,
						   &list);
    if (ret)
	return 0;

    if (list.len > 0 && list.val[0].length > 0)
	ret = decode_MS_UPN_SAN(list.val[0].data, list.val[0].length,
				upn, NULL);
    else
	ret = 1;
    hx509_free_octet_string_list(&list);

    return ret;
}
