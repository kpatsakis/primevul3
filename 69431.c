gstd_get_export_name(gss_name_t client)
{
	OM_uint32	maj;
	OM_uint32	min;
	gss_buffer_desc	buf;
	unsigned char   *bufp;
	unsigned char   nibble;
	char		*ret;
	size_t	  i, k;

	maj = gss_export_name(&min, client, &buf);
	GSTD_GSS_ERROR(maj, min, NULL, "gss_export_name");

	if ((ret = (char *)malloc(buf.length * 2 + 1)) == NULL) {
		LOG(LOG_ERR, ("unable to malloc"));
		gss_release_buffer(&min, &buf);
		return NULL;
	}

	for (bufp = buf.value, i = 0, k = 0; i < buf.length; i++) {
		nibble = bufp[i] >> 4;
		ret[k++] = "0123456789ABCDEF"[nibble];
		nibble = bufp[i] & 0x0f;
		ret[k++] = "0123456789ABCDEF"[nibble];
	}

	ret[k] = '\0';
	gss_release_buffer(&min, &buf);

	return ret;
}
