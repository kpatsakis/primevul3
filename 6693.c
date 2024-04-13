void gf_fs_send_update(GF_FilterSession *fsess, const char *fid, GF_Filter *filter, const char *name, const char *val, GF_EventPropagateType propagate_mask)
{
	GF_FilterUpdate *upd;
	u32 i, count;
	char *sep = NULL;
	Bool removed = GF_FALSE;
	if ((!fid && !filter) || !name) return;
	if (!fsess) {
		if (!filter) return;
		fsess = filter->session;
	}

	gf_mx_p(fsess->filters_mx);

	if (!filter) {
		GF_Filter *reg_filter = NULL;
		count = gf_list_count(fsess->filters);
		for (i=0; i<count; i++) {
			filter = gf_list_get(fsess->filters, i);
			if (filter->id && !strcmp(filter->id, fid)) {
				break;
			}
			if (filter->name && !strcmp(filter->name, fid)) {
				break;
			}
			if (!reg_filter && !strcmp(filter->freg->name, fid))
				reg_filter = filter;
			filter = NULL;
		}
		if (!filter)
			filter = reg_filter;
	}

	if (filter && filter->multi_sink_target)
		filter = filter->multi_sink_target;

	removed = (!filter || filter->removed || filter->finalized) ? GF_TRUE : GF_FALSE;
	gf_mx_v(fsess->filters_mx);

	if (removed) return;

	if (!val) {
		sep = strchr(name, fsess->sep_name);
		if (sep) sep[0] = 0;
	}

	//find arg and check if it is only a sync update - if so do it now
	i=0;
	while (filter->freg->args) {
		const GF_FilterArgs *a = &filter->freg->args[i];
		i++;
		if (!a || !a->arg_name) break;

		if ((a->flags & GF_FS_ARG_META) && !strcmp(a->arg_name, "*")) {
			continue;
		} else if (strcmp(a->arg_name, name)) {
			continue;
		}

		if (a->flags & GF_FS_ARG_UPDATE_SYNC) {
			gf_mx_p(filter->tasks_mx);
			gf_filter_update_arg_apply(filter, name, sep ? sep+1 : val, GF_TRUE);
			gf_mx_v(filter->tasks_mx);
			if (sep) sep[0] = fsess->sep_name;
			return;
		}
		break;
	}

	GF_SAFEALLOC(upd, GF_FilterUpdate);
	if (!val) {
		upd->name = gf_strdup(name);
		upd->val = sep ? gf_strdup(sep+1) : NULL;
		if (sep) sep[0] = fsess->sep_name;
	} else {
		upd->name = gf_strdup(name);
		upd->val = gf_strdup(val);
	}
	upd->recursive = propagate_mask;
	gf_fs_post_task(fsess, gf_filter_update_arg_task, filter, NULL, "update_arg", upd);
}