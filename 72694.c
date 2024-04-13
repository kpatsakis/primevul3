static Bool merge_parameter_set(GF_List *src, GF_List *dst, const char *name)
{
	u32 j, k;
	for (j=0; j<gf_list_count(src); j++) {
		Bool found = 0;
		GF_AVCConfigSlot *slc = gf_list_get(src, j);
		for (k=0; k<gf_list_count(dst); k++) {
			GF_AVCConfigSlot *slc_dst = gf_list_get(dst, k);
			if ( (slc->size==slc_dst->size) && !memcmp(slc->data, slc_dst->data, slc->size) ) {
				found = 1;
				break;
			}
		}
		if (!found) {
			return GF_FALSE;
		}
	}
	return GF_TRUE;
}
