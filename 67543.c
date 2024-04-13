X509_VERIFY_PARAM_set_trust(X509_VERIFY_PARAM *param, int trust)
{
	return X509_TRUST_set(&param->trust, trust);
}
