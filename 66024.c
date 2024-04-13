GF_Err segr_Write(GF_Box *s, GF_BitStream *bs)
{
	u32 i, k;
	FDSessionGroupBox *ptr = (FDSessionGroupBox *) s;
	if (!s) return GF_BAD_PARAM;

	gf_bs_write_u16(bs, ptr->num_session_groups);
	for (i=0; i<ptr->num_session_groups; i++) {
		gf_bs_write_u8(bs, ptr->session_groups[i].nb_groups);
		for (k=0; k<ptr->session_groups[i].nb_groups; k++) {
			gf_bs_write_u32(bs, ptr->session_groups[i].group_ids[k]);
		}

		gf_bs_write_u16(bs, ptr->session_groups[i].nb_channels);
		for (k=0; k<ptr->session_groups[i].nb_channels; k++) {
			gf_bs_write_u32(bs, ptr->session_groups[i].channels[k]);
		}
	}
	return GF_OK;
}
