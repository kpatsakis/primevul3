static GF_Filter *gf_fs_load_encoder(GF_FilterSession *fsess, const char *args)
{
	GF_Err e;
	char szCodec[3];
	char *cid, *sep;
	const GF_FilterRegister *candidate = NULL;
	u32 codecid=0;
	GF_Filter *filter;
	u32 i, count;
	GF_PropertyValue cap_in, cap_out;
	GF_List *blacklist = NULL;
	Bool ocap_excluded = GF_FALSE;
	szCodec[0] = 'c';
	szCodec[1] = fsess->sep_name;
	szCodec[2] = 0;

	cid = args ? strstr(args, szCodec) : NULL;
	if (!cid) {
		GF_LOG(GF_LOG_ERROR, GF_LOG_FILTER, ("Missing codec identifier in \"enc\" definition: %s\n", args ? args : "no arguments"));
		return NULL;
	}
	sep = strchr(cid, fsess->sep_args);
	if (sep) sep[0] = 0;

	codecid = gf_codecid_parse(cid+2);
#if 0
	if (codecid==GF_CODECID_NONE) {
		GF_LOG(GF_LOG_ERROR, GF_LOG_FILTER, ("Unrecognized codec identifier in \"enc\" definition: %s\n", cid));
		if (sep) sep[0] = fsess->sep_args;
		return NULL;
	}
#endif
	if (sep) sep[0] = fsess->sep_args;

	cap_in.type = GF_PROP_UINT;
	cap_in.value.uint = GF_CODECID_RAW;
	cap_out.type = GF_PROP_UINT;
	if (codecid==GF_CODECID_NONE) {
		cap_out.value.uint = GF_CODECID_RAW;
		ocap_excluded = GF_TRUE;
	} else {
		cap_out.value.uint = codecid;
	}

retry:
	count = gf_list_count(fsess->registry);
	for (i=0; i<count; i++) {
		const GF_FilterRegister *f_reg = gf_list_get(fsess->registry, i);
		if (blacklist && gf_list_find(blacklist, (void *) f_reg))
			continue;

		if ( gf_fs_check_filter_register_cap_ex(f_reg, GF_PROP_PID_CODECID, &cap_in, GF_PROP_PID_CODECID, &cap_out, GF_FALSE, ocap_excluded)) {
			if (!candidate || (candidate->priority>f_reg->priority))
				candidate = f_reg;
		}
	}
	if (!candidate) {
		GF_LOG(GF_LOG_ERROR, GF_LOG_FILTER, ("Cannot find any filter providing encoding for %s\n", cid));
		if (blacklist) gf_list_del(blacklist);
		return NULL;
	}
	filter = gf_filter_new(fsess, candidate, args, NULL, GF_FILTER_ARG_EXPLICIT, &e, NULL, GF_FALSE);
	if (!filter) {
		if (e==GF_NOT_SUPPORTED) {
			if (!blacklist) blacklist = gf_list_new();
			gf_list_add(blacklist, (void *) candidate);
			goto retry;
		}
		GF_LOG(GF_LOG_ERROR, GF_LOG_FILTER, ("Failed to load filter %s: %s\n", candidate->name, gf_error_to_string(e) ));
	} else {
		filter->encoder_stream_type = gf_codecid_type(codecid);
	}
	if (blacklist) gf_list_del(blacklist);
	return filter;
}