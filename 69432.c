gstd_get_mech(gss_OID mech_oid)
{
#ifdef HAVE_GSS_OID_TO_STR
	OM_uint32	maj;
	OM_uint32	min;
#endif
	gss_buffer_desc	buf;
	unsigned char   *bufp;
	unsigned char   nibble;
	char		*ret;
	size_t		i, k;

	if (mech_oid->length == sizeof(KNC_KRB5_MECH_OID) - 1 &&
	    memcmp(mech_oid->elements, KNC_KRB5_MECH_OID,
		   sizeof(KNC_KRB5_MECH_OID) - 1) == 0) {
		if ((ret = strdup("krb5")) == NULL) {
			LOG(LOG_ERR, ("unable to malloc"));
			return NULL;
		}
		return ret;
	}

#ifdef HAVE_GSS_OID_TO_STR
	maj = gss_oid_to_str(&min, mech_oid, &buf);
	if (maj != GSS_S_COMPLETE) {
		LOG(LOG_ERR, ("unable to display mechanism OID"));
		return NULL;
	}
	ret = strndup(buf.value, buf.length);
#else
	ret = strdup("");
#endif
	if (!ret)
		LOG(LOG_ERR, ("unable to malloc"));
	return ret;
}
