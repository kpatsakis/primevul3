Bool gf_fs_is_supported_mime(GF_FilterSession *fsess, const char *mime)
{
	u32 i, count;
	//first pass on explicit mimes
	count = gf_list_count(fsess->registry);
	for (i=0; i<count; i++) {
		u32 j;
		const GF_FilterRegister *freg = gf_list_get(fsess->registry, i);
		for (j=0; j<freg->nb_caps; j++) {
			const GF_FilterCapability *acap = &freg->caps[j];
			if (!(acap->flags & GF_CAPFLAG_INPUT)) continue;
			if (acap->code == GF_PROP_PID_MIME) {
				if (acap->val.value.string && strstr(acap->val.value.string, mime)) return GF_TRUE;
			}
		}
	}
	return GF_FALSE;
}