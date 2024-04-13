GF_Err avcc_Read(GF_Box *s, GF_BitStream *bs)
{
	u32 i, count;
	GF_AVCConfigurationBox *ptr = (GF_AVCConfigurationBox *)s;

	if (ptr->config) gf_odf_avc_cfg_del(ptr->config);
	ptr->config = gf_odf_avc_cfg_new();
	ptr->config->configurationVersion = gf_bs_read_u8(bs);
	ptr->config->AVCProfileIndication = gf_bs_read_u8(bs);
	ptr->config->profile_compatibility = gf_bs_read_u8(bs);
	ptr->config->AVCLevelIndication = gf_bs_read_u8(bs);
	if (ptr->type==GF_ISOM_BOX_TYPE_AVCC) {
		gf_bs_read_int(bs, 6);
	} else {
		ptr->config->complete_representation = gf_bs_read_int(bs, 1);
		gf_bs_read_int(bs, 5);
	}
	ptr->config->nal_unit_size = 1 + gf_bs_read_int(bs, 2);
	gf_bs_read_int(bs, 3);
	count = gf_bs_read_int(bs, 5);

	ptr->size -= 7; //including 2nd count

	for (i=0; i<count; i++) {
		GF_AVCConfigSlot *sl = (GF_AVCConfigSlot *) gf_malloc(sizeof(GF_AVCConfigSlot));
		sl->size = gf_bs_read_u16(bs);
		sl->data = (char *)gf_malloc(sizeof(char) * sl->size);
		gf_bs_read_data(bs, sl->data, sl->size);
		gf_list_add(ptr->config->sequenceParameterSets, sl);
		ptr->size -= 2+sl->size;
	}

	count = gf_bs_read_u8(bs);
	for (i=0; i<count; i++) {
		GF_AVCConfigSlot *sl = (GF_AVCConfigSlot *)gf_malloc(sizeof(GF_AVCConfigSlot));
		sl->size = gf_bs_read_u16(bs);
		if (gf_bs_available(bs) < sl->size) {
			gf_free(sl);
			GF_LOG(GF_LOG_ERROR, GF_LOG_CODING, ("AVCC: Not enough bits to parse. Aborting.\n"));
			return GF_ISOM_INVALID_FILE;
		}
		sl->data = (char *)gf_malloc(sizeof(char) * sl->size);
		gf_bs_read_data(bs, sl->data, sl->size);
		gf_list_add(ptr->config->pictureParameterSets, sl);
		ptr->size -= 2+sl->size;
	}

	if (ptr->type==GF_ISOM_BOX_TYPE_AVCC) {
		if (gf_avc_is_rext_profile(ptr->config->AVCProfileIndication)) {
			if (!ptr->size) {
#ifndef GPAC_DISABLE_AV_PARSERS
				AVCState avc;
				s32 idx, vui_flag_pos;
				GF_AVCConfigSlot *sl = (GF_AVCConfigSlot*)gf_list_get(ptr->config->sequenceParameterSets, 0);
				idx = gf_media_avc_read_sps(sl->data+1, sl->size-1, &avc, 0, (u32 *) &vui_flag_pos);
				if (idx>=0) {
					ptr->config->chroma_format = avc.sps[idx].chroma_format;
					ptr->config->luma_bit_depth = 8 + avc.sps[idx].luma_bit_depth_m8;
					ptr->config->chroma_bit_depth = 8 + avc.sps[idx].chroma_bit_depth_m8;
				}
#else
				/*set default values ...*/
				ptr->config->chroma_format = 1;
				ptr->config->luma_bit_depth = 8;
				ptr->config->chroma_bit_depth = 8;
#endif
				return GF_OK;
			}
			gf_bs_read_int(bs, 6);
			ptr->config->chroma_format = gf_bs_read_int(bs, 2);
			gf_bs_read_int(bs, 5);
			ptr->config->luma_bit_depth = 8 + gf_bs_read_int(bs, 3);
			gf_bs_read_int(bs, 5);
			ptr->config->chroma_bit_depth = 8 + gf_bs_read_int(bs, 3);

			count = gf_bs_read_int(bs, 8);
			ptr->size -= 4;
			if (count*2 > ptr->size) {
				GF_LOG(GF_LOG_WARNING, GF_LOG_CODING, ("AVCC: invalid numOfSequenceParameterSetExt value. Skipping.\n"));
				return GF_OK;
			}
			if (count) {
				ptr->config->sequenceParameterSetExtensions = gf_list_new();
				for (i=0; i<count; i++) {
					GF_AVCConfigSlot *sl = (GF_AVCConfigSlot *)gf_malloc(sizeof(GF_AVCConfigSlot));
					sl->size = gf_bs_read_u16(bs);
					if (gf_bs_available(bs) < sl->size) {
						gf_free(sl);
						GF_LOG(GF_LOG_ERROR, GF_LOG_CODING, ("AVCC: Not enough bits to parse. Aborting.\n"));
						return GF_ISOM_INVALID_FILE;
					}
					sl->data = (char *)gf_malloc(sizeof(char) * sl->size);
					gf_bs_read_data(bs, sl->data, sl->size);
					gf_list_add(ptr->config->sequenceParameterSetExtensions, sl);
					ptr->size -= sl->size + 2;
				}
			}
		}
	}
	return GF_OK;
}
