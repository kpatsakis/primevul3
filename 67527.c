X509_VERIFY_PARAM_get_count(void)
{
	int num = sizeof(default_table) / sizeof(X509_VERIFY_PARAM);
	if (param_table)
		num += sk_X509_VERIFY_PARAM_num(param_table);
	return num;
}
