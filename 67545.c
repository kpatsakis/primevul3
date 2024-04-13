int_x509_param_set1(char **pdest, size_t *pdestlen,  const char *src,
    size_t srclen)
{
	char *tmp;
	if (src) {
		if (srclen == 0) {
			if ((tmp = strdup(src)) == NULL)
				return 0;
			srclen = strlen(src);
		} else {
			if ((tmp = malloc(srclen)) == NULL)
				return 0;
			memcpy(tmp, src, srclen);
		}
	} else {
		tmp = NULL;
		srclen = 0;
	}
	if (*pdest)
		free(*pdest);
	*pdest = tmp;
	if (pdestlen)
		*pdestlen = srclen;
	return 1;
}
