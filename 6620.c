static GF_Filter *locate_alias_sink(GF_Filter *filter, const char *url, const char *mime_type)
{
	u32 i;
	for (i=0; i<filter->num_output_pids; i++) {
		u32 j;
		GF_FilterPid *pid = gf_list_get(filter->output_pids, i);
		for (j=0; j<pid->num_destinations; j++) {
			GF_Filter *f;
			GF_FilterPidInst *pidi = gf_list_get(pid->destinations, j);
			if (!pidi->filter) continue;
			if (pidi->filter->act_as_sink && pidi->filter->freg->use_alias
				&& pidi->filter->freg->use_alias(pidi->filter, url, mime_type)
			) {
				return pidi->filter;
			}
			//recursovely walk towards the sink
			f = locate_alias_sink(pidi->filter, url, mime_type);
			if (f) return f;
		}
	}
	return NULL;
}