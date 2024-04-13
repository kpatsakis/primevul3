X509_VERIFY_PARAM_free(X509_VERIFY_PARAM *param)
{
	if (param == NULL)
		return;
	x509_verify_param_zero(param);
	free(param->id);
	free(param);
}
