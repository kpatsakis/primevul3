pa_pw_or_afs3_salt(krb5_context context,
		   const krb5_principal client,
		   const AS_REQ *asreq,
		   struct pa_info_data *paid,
		   heim_octet_string *data)
{
    krb5_error_code ret;
    if (paid->etype == KRB5_ENCTYPE_NULL)
	return NULL;
    ret = set_paid(paid, context,
		   paid->etype,
		   paid->salt.salttype,
		   data->data,
		   data->length,
		   NULL);
    if (ret)
	return NULL;
    return paid;
}
