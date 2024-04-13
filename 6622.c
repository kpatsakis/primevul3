void gf_fs_push_arg(GF_FilterSession *session, const char *szArg, u32 was_found, u32 type)
{
	if (session->flags & GF_FS_FLAG_NO_ARG_CHECK)
		return;

	if (!was_found) {
		Bool afound = GF_FALSE;
		u32 k, acount;
		if (!session->parsed_args) session->parsed_args = gf_list_new();
		acount = gf_list_count(session->parsed_args);
		for (k=0; k<acount; k++) {
			GF_FSArgItem *ai = gf_list_get(session->parsed_args, k);
			if (!strcmp(ai->argname, szArg)) {
				afound = GF_TRUE;
				if ((ai->type==2) && (type==2) && (ai->found_type==1))
					ai->found_type = 0;
				break;
			}
		}
		if (!afound) {
			GF_FSArgItem *ai;
			GF_SAFEALLOC(ai, GF_FSArgItem);
			if (ai) {
				ai->argname = gf_strdup(szArg);
				ai->type = type;
				gf_list_add(session->parsed_args, ai );
			}
		}
	} else {
		u32 k, acount;
		Bool found = GF_FALSE;
		if (!session->parsed_args) session->parsed_args = gf_list_new();
		acount = gf_list_count(session->parsed_args);
		for (k=0; k<acount; k++) {
			GF_FSArgItem *ai = gf_list_get(session->parsed_args, k);
			if (!strcmp(ai->argname, szArg)) {
				ai->found_type = was_found;
				found = GF_TRUE;
				break;
			}
		}
		if (!found) {
			GF_FSArgItem *ai;
			GF_SAFEALLOC(ai, GF_FSArgItem);
			if (ai) {
				ai->argname = gf_strdup(szArg);
				ai->type = type;
				ai->found_type = was_found;
				gf_list_add(session->parsed_args, ai );
			}
		}
	}
}