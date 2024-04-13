X509_VERIFY_PARAM_table_cleanup(void)
{
	if (param_table)
		sk_X509_VERIFY_PARAM_pop_free(param_table,
		    X509_VERIFY_PARAM_free);
	param_table = NULL;
}
