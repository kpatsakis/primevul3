X509_VERIFY_PARAM_set1(X509_VERIFY_PARAM *to, const X509_VERIFY_PARAM *from)
{
	unsigned long save_flags = to->inh_flags;
	int ret;

	to->inh_flags |= X509_VP_FLAG_DEFAULT;
	ret = X509_VERIFY_PARAM_inherit(to, from);
	to->inh_flags = save_flags;
	return ret;
}
