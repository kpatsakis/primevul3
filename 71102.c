pa_etype_info(krb5_context context,
	      const krb5_principal client,
	      const AS_REQ *asreq,
	      struct pa_info_data *paid,
	      heim_octet_string *data)
{
    krb5_error_code ret;
    ETYPE_INFO e;
    size_t sz;
    size_t i, j;

    memset(&e, 0, sizeof(e));
    ret = decode_ETYPE_INFO(data->data, data->length, &e, &sz);
    if (ret)
	goto out;
    if (e.len == 0)
	goto out;
    for (j = 0; j < asreq->req_body.etype.len; j++) {
	for (i = 0; i < e.len; i++) {
	    if (asreq->req_body.etype.val[j] == e.val[i].etype) {
		krb5_salt salt;
		salt.salttype = KRB5_PW_SALT;
		if (e.val[i].salt == NULL)
		    ret = krb5_get_pw_salt(context, client, &salt);
		else {
		    salt.saltvalue = *e.val[i].salt;
		    ret = 0;
		}
		if (e.val[i].salttype)
		    salt.salttype = *e.val[i].salttype;
		if (ret == 0) {
		    ret = set_paid(paid, context, e.val[i].etype,
				   salt.salttype,
				   salt.saltvalue.data,
				   salt.saltvalue.length,
				   NULL);
		    if (e.val[i].salt == NULL)
			krb5_free_salt(context, salt);
		}
		if (ret == 0) {
		    free_ETYPE_INFO(&e);
		    return paid;
		}
	    }
	}
    }
 out:
    free_ETYPE_INFO(&e);
    return NULL;
}
