GF_Err chnl_box_read(GF_Box *s,GF_BitStream *bs)
{
	GF_ChannelLayoutBox *ptr = (GF_ChannelLayoutBox *) s;

	ISOM_DECREASE_SIZE(s, 1)
	ptr->layout.stream_structure = gf_bs_read_u8(bs);
	if (ptr->layout.stream_structure & 1) {
		ISOM_DECREASE_SIZE(s, 1)
		ptr->layout.definedLayout = gf_bs_read_u8(bs);
		if (ptr->layout.definedLayout) {
			u32 remain = (u32) ptr->size;
			if (ptr->layout.stream_structure & 2) remain--;
			ptr->layout.channels_count = 0;
			while (remain) {
				ISOM_DECREASE_SIZE(s, 1)
				ptr->layout.layouts[ptr->layout.channels_count].position = gf_bs_read_u8(bs);
				remain--;
				if (ptr->layout.layouts[ptr->layout.channels_count].position == 126) {
					ISOM_DECREASE_SIZE(s, 3)
					ptr->layout.layouts[ptr->layout.channels_count].azimuth = gf_bs_read_int(bs, 16);
					ptr->layout.layouts[ptr->layout.channels_count].elevation = gf_bs_read_int(bs, 8);
					remain-=3;
				}
			}
		} else {
			ISOM_DECREASE_SIZE(s, 8)
			ptr->layout.omittedChannelsMap = gf_bs_read_u64(bs);
		}
	}
	if (ptr->layout.stream_structure & 2) {
		ISOM_DECREASE_SIZE(s, 1)
		ptr->layout.object_count = gf_bs_read_u8(bs);
	}
	return GF_OK;
}