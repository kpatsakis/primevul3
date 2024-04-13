X509_VERIFY_PARAM *X509_VERIFY_PARAM_get0(int id)
{
	int num = sizeof(default_table) / sizeof(X509_VERIFY_PARAM);
	if (id < num)
		return default_table + id;
	return sk_X509_VERIFY_PARAM_value(param_table, id - num);
}
