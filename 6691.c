static void gf_fs_print_jsf_connection(GF_FilterSession *session, char *filter_name, GF_Filter *js_filter, void (*print_fn)(FILE *output, GF_SysPrintArgFlags flags, const char *fmt, ...) )
{
	GF_CapsBundleStore capstore;
	const char *js_name = NULL;
	GF_Err e=GF_OK;
	u32 i, j, count, nb_js_caps;
	GF_List *sources, *sinks;
	GF_FilterRegister loaded_freg;
	Bool has_output, has_input;

	if (!js_filter) {
		js_filter = gf_fs_load_filter(session, filter_name, &e);
		if (!js_filter) return;
	}

	js_name = strrchr(filter_name, '/');
	if (!js_name) js_name = strrchr(filter_name, '\\');
	if (js_name) js_name++;
	else js_name = filter_name;

	nb_js_caps = gf_filter_caps_bundle_count(js_filter->forced_caps, js_filter->nb_forced_caps);

	//fake a new register with only the caps set
	memset(&loaded_freg, 0, sizeof(GF_FilterRegister));
	loaded_freg.caps = js_filter->forced_caps;
	loaded_freg.nb_caps = js_filter->nb_forced_caps;

	has_output = gf_filter_has_out_caps(js_filter->forced_caps, js_filter->nb_forced_caps);
	has_input = gf_filter_has_in_caps(js_filter->forced_caps, js_filter->nb_forced_caps);

	memset(&capstore, 0, sizeof(GF_CapsBundleStore));
	sources = gf_list_new();
	sinks = gf_list_new();
	//edges for JS are for the unloaded JSF (eg accept anything, output anything).
	//we need to do a manual check
	count = gf_list_count(session->links);
	for (i=0; i<count; i++) {
		u32 nb_src_caps, k, l;
		Bool src_match = GF_FALSE;
		Bool sink_match = GF_FALSE;
		GF_FilterRegDesc *a_reg = gf_list_get(session->links, i);
		if (a_reg->freg == js_filter->freg) continue;

		//check which cap of this filter matches our destination
		nb_src_caps = gf_filter_caps_bundle_count(a_reg->freg->caps, a_reg->freg->nb_caps);
		for (k=0; k<nb_src_caps; k++) {
			for (l=0; l<nb_js_caps; l++) {
				s32 bundle_idx;
				u32 loaded_filter_only_flags = 0;
				u32 path_weight;
				if (has_input && !src_match) {
					path_weight = gf_filter_caps_to_caps_match(a_reg->freg, k, (const GF_FilterRegister *) &loaded_freg, NULL, &bundle_idx, l, &loaded_filter_only_flags, &capstore);
					if (path_weight && (bundle_idx == l))
						src_match = GF_TRUE;
				}
				if (has_output && !sink_match) {
					loaded_filter_only_flags = 0;
					path_weight = gf_filter_caps_to_caps_match(&loaded_freg, l, a_reg->freg, NULL, &bundle_idx, k, &loaded_filter_only_flags, &capstore);

					if (path_weight && (bundle_idx == k))
						sink_match = GF_TRUE;
				}
			}
			if (src_match && sink_match)
				break;
		}
		if (src_match) gf_list_add(sources, (void *) a_reg->freg);
		if (sink_match) gf_list_add(sinks, (void *) a_reg->freg);
	}

	for (i=0; i<2; i++) {
		GF_List *from = i ? sinks : sources;
		char *type = i ? "sinks" : "sources";

		count = gf_list_count(from);
		if (!count) {
			if (print_fn)
				print_fn(stderr, 1, "%s: no %s\n", js_name, type);
			else {
				GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("%s: no %s\n", type));
			}
			continue;
		}

		if (print_fn)
			print_fn(stderr, 1, "%s %s:", js_name, type);
		else {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("%s %s:", js_name, type));
		}
		for (j=0; j<count; j++) {
			GF_FilterRegister *a_reg = gf_list_get(from, j);
			if (print_fn)
				print_fn(stderr, 0, " %s", a_reg->name);
			else {
				GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" %s", a_reg->name));
			}
		}
		if (print_fn)
			print_fn(stderr, 0, "\n");
		else {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\n"));
		}
	}

	if (capstore.bundles_cap_found) gf_free(capstore.bundles_cap_found);
	if (capstore.bundles_in_ok) gf_free(capstore.bundles_in_ok);
	if (capstore.bundles_in_scores) gf_free(capstore.bundles_in_scores);
	gf_list_del(sources);
	gf_list_del(sinks);
}