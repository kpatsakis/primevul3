X509_VERIFY_PARAM_get0_peername(X509_VERIFY_PARAM *param)
{
	return param->id->peername;
}
