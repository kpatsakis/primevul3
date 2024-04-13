X509_VERIFY_PARAM_set_time(X509_VERIFY_PARAM *param, time_t t)
{
	param->check_time = t;
	param->flags |= X509_V_FLAG_USE_CHECK_TIME;
}
