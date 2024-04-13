X509_VERIFY_PARAM_set1_host(X509_VERIFY_PARAM *param,
    const char *name, size_t namelen)
{
	return int_x509_param_set_hosts(param->id, SET_HOST, name, namelen);
}
