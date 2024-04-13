X509_VERIFY_PARAM_set_hostflags(X509_VERIFY_PARAM *param, unsigned int flags)
{
	param->id->hostflags = flags;
}
