X509_VERIFY_PARAM_set1_ip_asc(X509_VERIFY_PARAM *param, const char *ipasc)
{
	unsigned char ipout[16];
	size_t iplen;

	iplen = (size_t)a2i_ipadd(ipout, ipasc);
	if (iplen == 0)
		return 0;
	return X509_VERIFY_PARAM_set1_ip(param, ipout, iplen);
}
