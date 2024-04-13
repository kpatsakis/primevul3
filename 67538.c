X509_VERIFY_PARAM_set1_name(X509_VERIFY_PARAM *param, const char *name)
{
	free(param->name);
	param->name = NULL;
	if (name == NULL)
		return 1;
	param->name = strdup(name);
	if (param->name)
		return 1;
	return 0;
}
