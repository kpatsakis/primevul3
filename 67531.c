X509_VERIFY_PARAM *X509_VERIFY_PARAM_lookup(const char *name)
{
	X509_VERIFY_PARAM pm;
	unsigned int i, limit;

	pm.name = (char *)name;
	if (param_table) {
		size_t idx;
		if ((idx = sk_X509_VERIFY_PARAM_find(param_table, &pm)) != -1)
			return sk_X509_VERIFY_PARAM_value(param_table, idx);
	}

	limit = sizeof(default_table) / sizeof(X509_VERIFY_PARAM);
	for (i = 0; i < limit; i++) {
		if (strcmp(default_table[i].name, name) == 0) {
			return &default_table[i];
		}
	}
	return NULL;
}
