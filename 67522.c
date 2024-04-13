X509_VERIFY_PARAM_clear_flags(X509_VERIFY_PARAM *param, unsigned long flags)
{
	param->flags &= ~flags;
	return 1;
}
