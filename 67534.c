X509_VERIFY_PARAM_set1_email(X509_VERIFY_PARAM *param,  const char *email,
    size_t emaillen)
{
	return int_x509_param_set1(&param->id->email, &param->id->emaillen,
	    email, emaillen);
}
