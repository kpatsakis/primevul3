pa_etype_info2(krb5_context context,
	       const krb5_principal client,
	       const AS_REQ *asreq,
	       struct pa_info_data *paid,
	       heim_octet_string *data)
{
    krb5_error_code ret;
    ETYPE_INFO2 e;
    size_t sz;
    size_t i, j;

    memset(&e, 0, sizeof(e));
    ret = decode_ETYPE_INFO2(data->data, data->length, &e, &sz);
    if (ret)
	goto out;
    if (e.len == 0)
	goto out;
    for (j = 0; j < asreq->req_body.etype.len; j++) {
	for (i = 0; i < e.len; i++) {
	    if (asreq->req_body.etype.val[j] == e.val[i].etype) {
		krb5_salt salt;
		if (e.val[i].salt == NULL)
		    ret = krb5_get_pw_salt(context, client, &salt);
		else {
		    salt.saltvalue.data = *e.val[i].salt;
		    salt.saltvalue.length = strlen(*e.val[i].salt);
		    ret = 0;
		}
		if (ret == 0)
		    ret = set_paid(paid, context, e.val[i].etype,
				   KRB5_PW_SALT,
				   salt.saltvalue.data,
				   salt.saltvalue.length,
				   e.val[i].s2kparams);
		if (e.val[i].salt == NULL)
		    krb5_free_salt(context, salt);
		if (ret == 0) {
		    free_ETYPE_INFO2(&e);
		    return paid;
		}
	    }
	}
    }
 out:
    free_ETYPE_INFO2(&e);
    return NULL;
}
