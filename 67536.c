X509_VERIFY_PARAM_set1_ip(X509_VERIFY_PARAM *param, const unsigned char *ip,
    size_t iplen)
{
	if (iplen != 0 && iplen != 4 && iplen != 16)
		return 0;
	return int_x509_param_set1((char **)&param->id->ip, &param->id->iplen,
	    (char *)ip, iplen);
}
