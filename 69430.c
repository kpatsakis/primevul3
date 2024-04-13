gstd_get_display_name(gss_name_t client)
{
	OM_uint32	maj;
	OM_uint32	min;
	gss_buffer_desc	buf;
	char		*ret;

	maj = gss_display_name(&min, client, &buf, NULL);
	GSTD_GSS_ERROR(maj, min, NULL, "gss_display_name");

	if ((ret = (char *)malloc(buf.length + 1)) == NULL) {
		LOG(LOG_ERR, ("unable to malloc"));
		gss_release_buffer(&min, &buf);
		return NULL;
	}

	memcpy(ret, buf.value, buf.length);
	ret[buf.length] = '\0';

	gss_release_buffer(&min, &buf);

	return ret;
}
