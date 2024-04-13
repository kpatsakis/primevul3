GF_Filter *gf_fs_load_source_dest_internal(GF_FilterSession *fsess, const char *url, const char *user_args, const char *parent_url, GF_Err *err, GF_Filter *filter, GF_Filter *dst_filter, Bool for_source, Bool no_args_inherit, Bool *probe_only)
{
	GF_FilterProbeScore score = GF_FPROBE_NOT_SUPPORTED;
	GF_FilterRegister *candidate_freg=NULL;
	GF_Filter *alias_for_filter = NULL;
	const GF_FilterArgs *src_dst_arg=NULL;
	u32 i, count, user_args_len, arg_type;
	char szForceReg[20];
	char szMime[50];
	GF_Err e;
	const char *force_freg = NULL;
	char *sURL, *mime_type, *args, *sep;
	char szExt[50];
	Bool free_url=GF_FALSE;
	memset(szExt, 0, sizeof(szExt));

	if (!url) {
		if (err) *err = GF_BAD_PARAM;
		return NULL;
	}
	if (err) *err = GF_OK;

	mime_type = NULL;
	//destination, extract mime from arguments
	if (!for_source) {
		sprintf(szMime, "%cmime=", fsess->sep_args);
		mime_type = strstr(url, szMime);
		if (!mime_type && user_args)
			mime_type = strstr(user_args, szMime);

		if (mime_type) {
			strncpy(szMime, mime_type+6, 49);
			szMime[49]=0;
			sep = strchr(szMime, fsess->sep_args);
			if (sep) sep[0] = 0;
			mime_type = szMime;
		}
	}
	sURL = NULL;
	if (filter) {
		sURL = (char *) url;
	} else {
		Bool is_local;
		/*used by GUIs scripts to skip URL concatenation*/
		if (!strncmp(url, "gpac://", 7)) sURL = gf_strdup(url+7);
		/*opera-style localhost URLs*/
		else if (!strncmp(url, "file://localhost", 16)) sURL = gf_strdup(url+16);
		else if (parent_url) sURL = gf_url_concatenate(parent_url, url);

		/*path absolute*/
		if (!sURL) sURL = gf_strdup(url);
		free_url=GF_TRUE;

		if (!strncmp(sURL, "gpac://", 7)) {
			u32 ulen = (u32) strlen(sURL+7);
			memmove(sURL, sURL+7, ulen);
			sURL[ulen]=0;
		}
		//remove any filter arguments in URL before checking if it is local
		//not doing so will lead wrong result if one argument is a URL (eg ":#BUrl=http://")
		sep = (char *) gf_fs_path_escape_colon(fsess, sURL);
		if (sep) sep[0] = 0;
		is_local = gf_url_is_local(sURL);
		if (sep) sep[0] = fsess->sep_args;

		if (for_source && is_local && !strstr(sURL, "isobmff://")) {
			char *frag_par, *cgi, *ext_start;
			char f_c=0;
			gf_url_to_fs_path(sURL);
			sep = (char *)gf_fs_path_escape_colon(fsess, sURL);
			if (sep) sep[0] = 0;

			ext_start = gf_file_ext_start(sURL);
			if (!ext_start) ext_start = sURL;
			frag_par = strchr(ext_start, '#');
			cgi = strchr(ext_start, '?');
			if (frag_par && cgi && (cgi<frag_par))
				frag_par = cgi;

			if (frag_par) {
				f_c = frag_par[0];
				frag_par[0] = 0;
			}

			if (strcmp(sURL, "null") && strncmp(sURL, "rand", 4) && strcmp(sURL, "-") && strcmp(sURL, "stdin") && ! gf_file_exists(sURL)) {
				char szPath[GF_MAX_PATH];
				Bool try_js = gf_fs_solve_js_script(szPath, sURL, NULL);
				if (sep) sep[0] = fsess->sep_args;
				if (frag_par) frag_par[0] = f_c;
				gf_free(sURL);

				if (try_js) {
					if (!strncmp(url, "gpac://", 7)) url += 7;
					filter = gf_fs_load_filter(fsess, url, err);

					if (filter) {
						//for link resolution
						if (dst_filter && for_source)	{
							if (gf_list_find(filter->destination_links, dst_filter)<0)
								gf_list_add(filter->destination_links, dst_filter);
							//to remember our connection target
							filter->target_filter = dst_filter;
						}
					}
					return filter;
				}

				if (err) *err = GF_URL_ERROR;
				return NULL;
			}
			if (frag_par) frag_par[0] = f_c;
			if (sep) sep[0] = fsess->sep_args;
		}
	}
	sep = (char *)gf_fs_path_escape_colon(fsess, sURL);

	sprintf(szForceReg, "gfreg%c", fsess->sep_name);
	force_freg = NULL;
	if (sep) {
		sep[0] = 0;
		force_freg = strstr(sep+1, szForceReg);
	}
	if (!force_freg && user_args) {
		force_freg = strstr(user_args, szForceReg);
	}
	if (force_freg)
		force_freg += 6;


	if (!for_source && dst_filter) {
		alias_for_filter = locate_alias_sink(dst_filter, sURL, mime_type);
		if (alias_for_filter) {
			candidate_freg = (GF_FilterRegister *) alias_for_filter->freg;
		}
	}

restart:
	//check all our registered filters
	count = gf_list_count(fsess->registry);
	if (candidate_freg) count = 0;
	for (i=0; i<count; i++) {
		u32 j;
		GF_FilterProbeScore s;
		GF_FilterRegister *freg = gf_list_get(fsess->registry, i);
		if (! freg->probe_url) continue;
		if (force_freg && strncmp(force_freg, freg->name, strlen(freg->name))) continue;
		if (! freg->args) continue;
		if (filter && (gf_list_find(filter->blacklisted, freg) >=0)) continue;

		j=0;
		while (freg->args) {
			src_dst_arg = &freg->args[j];
			if (!src_dst_arg || !src_dst_arg->arg_name) {
				src_dst_arg=NULL;
				break;
			}
			if (for_source && !strcmp(src_dst_arg->arg_name, "src")) break;
			else if (!for_source && !strcmp(src_dst_arg->arg_name, "dst") && !(src_dst_arg->flags&GF_FS_ARG_SINK_ALIAS)) break;
			src_dst_arg = NULL;
			j++;
		}
		if (!src_dst_arg)
			continue;

		s = freg->probe_url(sURL, mime_type);
		/* destination meta filter: change GF_FPROBE_SUPPORTED to GF_FPROBE_MAYBE_SUPPORTED for internal mux formats
		in order to avoid always giving the hand to the meta filter*/
		if (!for_source && (s == GF_FPROBE_SUPPORTED) && (freg->flags & GF_FS_REG_META)) {
			s = probe_meta_check_builtin_format(fsess, freg, sURL, mime_type, sep ? sep+1 : NULL);
		}
		//higher score, use this new registry
		if (s > score) {
			candidate_freg = freg;
			score = s;
		}
		//same score and higher priority (0 being highest), use this new registry
		else if ((s == score) && candidate_freg && (freg->priority<candidate_freg->priority) ) {
			candidate_freg = freg;
			score = s;
		}
	}
	if (probe_only) {
		*probe_only = candidate_freg ? GF_TRUE : GF_FALSE;
		if (free_url)
			gf_free(sURL);
		if (err) *err = GF_OK;
		return NULL;
	}

	if (!candidate_freg) {
		if (force_freg) {
			GF_LOG(GF_LOG_INFO, GF_LOG_FILTER, ("No source filter named %s found, retrying without forcing registry\n", force_freg));
			force_freg = NULL;
			goto restart;
		}
		if (free_url)
			gf_free(sURL);
		if (err) *err = GF_NOT_SUPPORTED;
		if (filter) filter->finalized = GF_TRUE;
		return NULL;
	}
	if (sep) sep[0] = fsess->sep_args;

	user_args_len = user_args ? (u32) strlen(user_args) : 0;
	args = gf_malloc(sizeof(char)*(5+strlen(sURL) + (user_args_len ? user_args_len + 8/*for potential :gpac: */  :0) ) );

	sprintf(args, "%s%c", for_source ? "src" : "dst", fsess->sep_name);
	strcat(args, sURL);
	if (user_args_len) {
		if (fsess->sep_args==':') strcat(args, ":gpac:");
		else {
			char szSep[2];
			szSep[0] = fsess->sep_args;
			szSep[1] = 0;
			strcat(args, szSep);
		}
		strcat(args, user_args);
	}

	e = GF_OK;
	arg_type = GF_FILTER_ARG_EXPLICIT_SINK;
	if (for_source) {
		if (no_args_inherit) arg_type = GF_FILTER_ARG_EXPLICIT_SOURCE_NO_DST_INHERIT;
		else arg_type = GF_FILTER_ARG_EXPLICIT_SOURCE;
	}

	if (!for_source && !alias_for_filter && candidate_freg->use_alias) {
		u32 fcount = gf_list_count(fsess->filters);
		for (i=0; i<fcount; i++) {
			GF_Filter *f = gf_list_get(fsess->filters, i);
			if (f->freg != candidate_freg) continue;
			if (f->freg->use_alias(f, sURL, mime_type)) {
				alias_for_filter = f;
				break;
			}
		}
	}

	if (!filter) {
		filter = gf_filter_new(fsess, candidate_freg, args, NULL, arg_type, err, alias_for_filter, GF_FALSE);
	} else {
        //destroy underlying JS object - gf_filter_new_finalize always reassign it to JS_UNDEFINED
#ifdef GPAC_HAS_QJS
        jsfs_on_filter_destroyed(filter);
#endif
		if (filter->session->on_filter_create_destroy)
			filter->session->on_filter_create_destroy(filter->session->rt_udta, filter, GF_TRUE);
			
        filter->freg = candidate_freg;
		e = gf_filter_new_finalize(filter, args, arg_type);
		if (err) *err = e;
	}

	if (free_url)
		gf_free(sURL);

	if (filter) {
		if (filter->src_args) gf_free(filter->src_args);
		filter->src_args = args;
		//for link resolution
		if (dst_filter && for_source)	{
			if (gf_list_find(filter->destination_links, dst_filter)<0)
				gf_list_add(filter->destination_links, dst_filter);
			//to remember our connection target
			filter->target_filter = dst_filter;
		}
	} else {
		gf_free(args);
	}

	if (!e && filter && !filter->num_output_pids && for_source)
		gf_filter_post_process_task(filter);

	return filter;
}