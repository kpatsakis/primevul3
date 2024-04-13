uint32_t GPMF_ExpandComplexTYPE(char *src, uint32_t srcsize, char *dst, uint32_t *dstsize)
{
	uint32_t i = 0, k = 0, count = 0;

	while (i<srcsize && k<*dstsize)
	{
		if (src[i] == '[' && i>0)
		{
			int j = 1;
			count = 0;
			while (src[i + j] >= '0' && src[i + j] <= '9')
			{
				count *= 10;
				count += src[i + j] - '0';
				j++;
			}

			if (count > 1)
			{
				uint32_t l;
				for (l = 1; l<count; l++)
				{
					dst[k] = src[i - 1];
					k++;
				}
			}
			i += j;
			if (src[i] == ']') i++;
		}
		else
		{
			dst[k] = src[i];
			if (dst[k] == 0) break;
			i++, k++;
		}
	}

	if (k >= *dstsize)
		return GPMF_ERROR_MEMORY; // bad structure formed

	dst[k] = 0;
	*dstsize = k;

	return GPMF_OK;
}
