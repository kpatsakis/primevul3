X509_VERIFY_PARAM_new(void)
{
	X509_VERIFY_PARAM *param;
	X509_VERIFY_PARAM_ID *paramid;
	param = calloc(1, sizeof(X509_VERIFY_PARAM));
	if (param == NULL)
		return NULL;
	paramid = calloc (1, sizeof(X509_VERIFY_PARAM_ID));
	if (paramid == NULL) {
		free(param);
		return NULL;
	}
	param->id = paramid;
	x509_verify_param_zero(param);
	return param;
}
