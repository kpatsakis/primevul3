X509_VERIFY_PARAM_add0_table(X509_VERIFY_PARAM *param)
{
	X509_VERIFY_PARAM *ptmp;
	if (!param_table) {
		param_table = sk_X509_VERIFY_PARAM_new(param_cmp);
		if (!param_table)
			return 0;
	} else {
		size_t idx;

		if ((idx = sk_X509_VERIFY_PARAM_find(param_table, param))
		    != -1) {
			ptmp = sk_X509_VERIFY_PARAM_value(param_table,
			    idx);
			X509_VERIFY_PARAM_free(ptmp);
			(void)sk_X509_VERIFY_PARAM_delete(param_table,
			    idx);
		}
	}
	if (!sk_X509_VERIFY_PARAM_push(param_table, param))
		return 0;
	return 1;
}
