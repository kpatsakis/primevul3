static SAPType is_sample_idr(GF_ISOSample *sample, GF_MPEGVisualSampleEntryBox *entry)
{
	Bool is_hevc = GF_FALSE;
	u32 nalu_size_field = 0;
	GF_BitStream *bs;
	if (entry->avc_config && entry->avc_config->config) nalu_size_field = entry->avc_config->config->nal_unit_size;
	else if (entry->svc_config && entry->svc_config->config) nalu_size_field = entry->svc_config->config->nal_unit_size;
	else if (entry->mvc_config && entry->mvc_config->config) nalu_size_field = entry->mvc_config->config->nal_unit_size;
	else if (entry->hevc_config && entry->hevc_config->config) {
		nalu_size_field = entry->hevc_config->config->nal_unit_size;
		is_hevc = GF_TRUE;
	}
	else if (entry->lhvc_config && entry->lhvc_config->config) {
		nalu_size_field = entry->lhvc_config->config->nal_unit_size;
		is_hevc = GF_TRUE;
	}
	if (!nalu_size_field) return RAP_NO;

	bs = gf_bs_new(sample->data, sample->dataLength, GF_BITSTREAM_READ);
	if (!bs) return RAP_NO;

	while (gf_bs_available(bs)) {
		u8 nal_type;
		u32 size = gf_bs_read_int(bs, 8*nalu_size_field);

		if (is_hevc) {
#ifndef GPAC_DISABLE_HEVC
			u16 nal_hdr = gf_bs_read_u16(bs);
			nal_type = (nal_hdr&0x7E00) >> 9;

			switch (nal_type) {
			case GF_HEVC_NALU_SLICE_CRA:
				gf_bs_del(bs);
				return SAP_TYPE_3;
			case GF_HEVC_NALU_SLICE_IDR_N_LP:
			case GF_HEVC_NALU_SLICE_BLA_N_LP:
				gf_bs_del(bs);
				return SAP_TYPE_1;
			case GF_HEVC_NALU_SLICE_IDR_W_DLP:
			case GF_HEVC_NALU_SLICE_BLA_W_DLP:
			case GF_HEVC_NALU_SLICE_BLA_W_LP:
				gf_bs_del(bs);
				return SAP_TYPE_2;
			case GF_HEVC_NALU_ACCESS_UNIT:
			case GF_HEVC_NALU_FILLER_DATA:
			case GF_HEVC_NALU_SEI_PREFIX:
			case GF_HEVC_NALU_VID_PARAM:
			case GF_HEVC_NALU_SEQ_PARAM:
			case GF_HEVC_NALU_PIC_PARAM:
				break;
			default:
				gf_bs_del(bs);
				return RAP_NO;
			}
			gf_bs_skip_bytes(bs, size - 2);
#endif
		} else {
			u8 nal_hdr = gf_bs_read_u8(bs);
			nal_type = nal_hdr & 0x1F;

			switch (nal_type) {
			/*			case GF_AVC_NALU_SEQ_PARAM:
						case GF_AVC_NALU_PIC_PARAM:
						case GF_AVC_NALU_SEQ_PARAM_EXT:
						case GF_AVC_NALU_SVC_SUBSEQ_PARAM:
		*/			case GF_AVC_NALU_IDR_SLICE:
				gf_bs_del(bs);
				return SAP_TYPE_1;
			case GF_AVC_NALU_ACCESS_UNIT:
			case GF_AVC_NALU_FILLER_DATA:
			case GF_AVC_NALU_SEI:
				break;
			default:
				gf_bs_del(bs);
				return RAP_NO;
			}
			gf_bs_skip_bytes(bs, size - 1);
		}
	}
	gf_bs_del(bs);
	return RAP_NO;
}
