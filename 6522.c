GF_Err afrt_box_read(GF_Box *s, GF_BitStream *bs)
{
	unsigned int i;
	GF_AdobeFragmentRunTableBox *ptr = (GF_AdobeFragmentRunTableBox *)s;

	ISOM_DECREASE_SIZE(ptr, 5)
	ptr->timescale = gf_bs_read_u32(bs);

	ptr->quality_entry_count = gf_bs_read_u8(bs);
	if (ptr->size < ptr->quality_entry_count)
		return GF_ISOM_INVALID_FILE;

	for (i=0; i<ptr->quality_entry_count; i++) {
		int j=0;
		u32 tmp_strsize=(u32)ptr->size-8;
		char *tmp_str = (char*) gf_malloc(tmp_strsize);
		if (!tmp_str) return GF_OUT_OF_MEM;
		while (tmp_strsize) {
			tmp_str[j] = gf_bs_read_u8(bs);
			tmp_strsize--;
			if (!tmp_str[j])
				break;
			j++;
		}
		ISOM_DECREASE_SIZE(ptr, j)
		gf_list_insert(ptr->quality_segment_url_modifiers, tmp_str, i);
	}

	ptr->fragment_run_entry_count = gf_bs_read_u32(bs);
	if (ptr->size / 16 < ptr->fragment_run_entry_count)
		return GF_ISOM_INVALID_FILE;
	for (i=0; i<ptr->fragment_run_entry_count; i++) {
		GF_AdobeFragmentRunEntry *fre = gf_malloc(sizeof(GF_AdobeFragmentRunEntry));
		if (!fre) return GF_OUT_OF_MEM;
		ISOM_DECREASE_SIZE(ptr, 16)
		fre->first_fragment = gf_bs_read_u32(bs);
		fre->first_fragment_timestamp = gf_bs_read_u64(bs);
		fre->fragment_duration = gf_bs_read_u32(bs);
		if (!fre->fragment_duration) {
			ISOM_DECREASE_SIZE(ptr, 1)
			fre->discontinuity_indicator = gf_bs_read_u8(bs);
		}
		gf_list_insert(ptr->fragment_run_entry_table, fre, i);
	}

	return GF_OK;
}