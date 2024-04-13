void gf_fs_print_all_connections(GF_FilterSession *session, char *filter_name, void (*print_fn)(FILE *output, GF_SysPrintArgFlags flags, const char *fmt, ...) )
{
	Bool found = GF_FALSE;
	GF_List *done;
	u32 i, j, count;
	u32 llev = gf_log_get_tool_level(GF_LOG_FILTER);

	gf_log_set_tool_level(GF_LOG_FILTER, GF_LOG_INFO);
	//load JS to inspect its connections
	if (filter_name && strstr(filter_name, ".js")) {
		gf_fs_print_jsf_connection(session, filter_name, NULL, print_fn);
		gf_log_set_tool_level(GF_LOG_FILTER, llev);
		return;
	}
	done = gf_list_new();
	count = gf_list_count(session->links);

	for (i=0; i<count; i++) {
		const GF_FilterRegDesc *src = gf_list_get(session->links, i);
		if (filter_name && strcmp(src->freg->name, filter_name))
			continue;

		if (!src->nb_edges) {
			if (print_fn)
				print_fn(stderr, 1, "%s: no sources\n", src->freg->name);
			else {
				GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("%s: no sources\n", src->freg->name));
			}
			continue;
		}
		found = GF_TRUE;
		if (print_fn)
			print_fn(stderr, 1, "%s sources:", src->freg->name);
		else {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("%s sources:", src->freg->name));
		}

		for (j=0; j<src->nb_edges; j++) {
			if (gf_list_find(done, (void *) src->edges[j].src_reg->freg->name)<0) {
				if (print_fn)
					print_fn(stderr, 0, " %s", src->edges[j].src_reg->freg->name);
				else {
					GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" %s", src->edges[j].src_reg->freg->name));
				}
				gf_list_add(done, (void *) src->edges[j].src_reg->freg->name);
			}
		}
		if (print_fn)
			print_fn(stderr, 0, "\n");
		else {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("\n"));
		}
		gf_list_reset(done);
	}

	if (found && filter_name) {
		if (print_fn)
			print_fn(stderr, 1, "%s sinks:", filter_name);
		else {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, ("%s sinks:", filter_name));
		}
		count = gf_list_count(session->links);
		for (i=0; i<count; i++) {
			const GF_FilterRegDesc *src = gf_list_get(session->links, i);
			if (!strcmp(src->freg->name, filter_name)) {
				if (!(src->freg->flags & GF_FS_REG_EXPLICIT_ONLY) || !(src->freg->flags & GF_FS_REG_ALLOW_CYCLIC))
					continue;
			}

			for (j=0; j<src->nb_edges; j++) {
				if (strcmp(src->edges[j].src_reg->freg->name, filter_name)) continue;

				if (gf_list_find(done, (void *) src->freg->name)<0) {
					if (print_fn)
						print_fn(stderr, 0, " %s", src->freg->name);
					else {
						GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" %s", src->freg->name));
					}
					gf_list_add(done, (void *) src->freg->name);
				}
			}
			gf_list_reset(done);
		}
		if (print_fn)
			print_fn(stderr, 1, " \n");
		else {
			GF_LOG(GF_LOG_INFO, GF_LOG_APP, (" \n"));
		}
	}

	if (!found && filter_name) {
		GF_Err e = GF_OK;
		GF_Filter *f = gf_fs_load_filter(session, filter_name, &e);
		if (f) {
			gf_fs_print_jsf_connection(session, filter_name, f, print_fn);
		}
		else if (print_fn)
			print_fn(stderr, 1, "%s filter not found\n", filter_name);
		else {
			GF_LOG(GF_LOG_ERROR, GF_LOG_APP, ("%s filter not found\n", filter_name));
		}
	}
	gf_list_del(done);
	gf_log_set_tool_level(GF_LOG_FILTER, llev);
}