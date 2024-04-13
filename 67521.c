X509_VERIFY_PARAM_add1_host(X509_VERIFY_PARAM *param,
    const char *name, size_t namelen)
{
	return int_x509_param_set_hosts(param->id, ADD_HOST, name, namelen);
}
