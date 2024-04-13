param_cmp(const X509_VERIFY_PARAM * const *a,
    const X509_VERIFY_PARAM * const *b)
{
	return strcmp((*a)->name, (*b)->name);
}
