static int asn1_encode_se_info(sc_context_t *ctx,
		struct sc_pkcs15_sec_env_info **se, size_t se_num,
		unsigned char **buf, size_t *bufsize, int depth)
{
	unsigned char *ptr = NULL, *out = NULL, *p;
	size_t ptrlen = 0, outlen = 0, idx;
	int ret;

	for (idx=0; idx < se_num; idx++)   {
		struct sc_asn1_entry asn1_se[2];
		struct sc_asn1_entry asn1_se_info[4];

		sc_copy_asn1_entry(c_asn1_se, asn1_se);
		sc_copy_asn1_entry(c_asn1_se_info, asn1_se_info);

		sc_format_asn1_entry(asn1_se_info + 0, &se[idx]->se, NULL, 1);
		if (sc_valid_oid(&se[idx]->owner))
			sc_format_asn1_entry(asn1_se_info + 1, &se[idx]->owner, NULL, 1);
		if (se[idx]->aid.len)
			sc_format_asn1_entry(asn1_se_info + 2, &se[idx]->aid.value, &se[idx]->aid.len, 1);
		sc_format_asn1_entry(asn1_se + 0, asn1_se_info, NULL, 1);

		ret = sc_asn1_encode(ctx, asn1_se, &ptr, &ptrlen);
		if (ret != SC_SUCCESS)
			goto err;

		if (!ptrlen)
			continue;
		p = (unsigned char *) realloc(out, outlen + ptrlen);
		if (!p)   {
			ret = SC_ERROR_OUT_OF_MEMORY;
			goto err;
		}
		out = p;
		memcpy(out + outlen, ptr, ptrlen);
		outlen += ptrlen;
		free(ptr);
		ptr = NULL;
		ptrlen = 0;
	}

	*buf = out;
	*bufsize = outlen;
	ret = SC_SUCCESS;
err:
	if (ret != SC_SUCCESS && out != NULL)
		free(out);
	return ret;
}
