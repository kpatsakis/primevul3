gstd_initiate(const char *hostname, const char *service, const char *princ,
	      int fd)
{
	struct gstd_tok	*tok;
	gss_ctx_id_t	ctx = GSS_C_NO_CONTEXT;
	gss_buffer_desc	in, out;
	gss_OID		type;
	OM_uint32	maj, min;
	gss_buffer_desc	name;
	gss_name_t	server;
	int		ret;

	if (!princ) {
		if ((name.value = malloc(strlen(service) + strlen(hostname)
					 + 2)) == NULL) {
			LOG(LOG_ERR, ("unable to malloc service name"));
			return NULL;
		}

		name.length = sprintf((char *)name.value, "%s@%s",
				      service, hostname);
		LOG(LOG_DEBUG, ("going to get tickets for: %s",
		    (char *)name.value));
		fflush(stderr);
		if (!name.value)
			return NULL;
		type = GSS_C_NT_HOSTBASED_SERVICE;
	} else {
		name.value = (char *) princ;
		name.length = strlen(princ);
		type = (gss_OID) GSS_C_NO_OID;
	}

	maj = gss_import_name(&min, &name, type, &server);
	GSTD_GSS_ERROR(maj, min, NULL, "gss_import_name");

	in.length = 0;
	out.length = 0;

again:
	maj = gss_init_sec_context(&min, GSS_C_NO_CREDENTIAL, &ctx, server,
	    GSS_C_NO_OID, GSS_C_MUTUAL_FLAG | GSS_C_SEQUENCE_FLAG, 0,
	    GSS_C_NO_CHANNEL_BINDINGS, &in, NULL, &out, NULL, NULL);

	if (out.length && write_packet(fd, &out))
		return NULL;

	GSTD_GSS_ERROR(maj, min, NULL, "gss_init_sec_context");

	if (GSS_ERROR(maj) && ctx != GSS_C_NO_CONTEXT) {
		gss_delete_sec_context(&min, &ctx, GSS_C_NO_BUFFER);
		return NULL;
	}

	if (maj & GSS_S_CONTINUE_NEEDED) {
		LOG(LOG_DEBUG, ("continuing gstd_initiate"));
		while ((ret = read_packet(fd, &in, 0, 0)) == -2)
			;

		if (ret < 1) {
			LOG(LOG_ERR, ("continuation failed"));
			return NULL;
		}

		goto again;
	}

	LOG(LOG_DEBUG, ("authenticated"));
	SETUP_GSTD_TOK(tok, ctx, fd, "gstd_connect");
	return tok;
}
