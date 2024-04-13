set_paid(struct pa_info_data *paid, krb5_context context,
	 krb5_enctype etype,
	 krb5_salttype salttype, void *salt_string, size_t salt_len,
	 krb5_data *s2kparams)
{
    paid->etype = etype;
    paid->salt.salttype = salttype;
    paid->salt.saltvalue.data = malloc(salt_len + 1);
    if (paid->salt.saltvalue.data == NULL) {
	krb5_clear_error_message(context);
	return ENOMEM;
    }
    memcpy(paid->salt.saltvalue.data, salt_string, salt_len);
    ((char *)paid->salt.saltvalue.data)[salt_len] = '\0';
    paid->salt.saltvalue.length = salt_len;
    if (s2kparams) {
	krb5_error_code ret;

	ret = krb5_copy_data(context, s2kparams, &paid->s2kparams);
	if (ret) {
	    krb5_clear_error_message(context);
	    krb5_free_salt(context, paid->salt);
	    return ret;
	}
    } else
	paid->s2kparams = NULL;

    return 0;
}
