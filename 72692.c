static u32 merge_avc_config(GF_ISOFile *dest, u32 tk_id, GF_ISOFile *orig, u32 src_track, Bool force_cat)
{
	GF_AVCConfig *avc_src, *avc_dst;
	u32 dst_tk = gf_isom_get_track_by_id(dest, tk_id);

	avc_src = gf_isom_avc_config_get(orig, src_track, 1);
	avc_dst = gf_isom_avc_config_get(dest, dst_tk, 1);

	if (avc_src->AVCLevelIndication!=avc_dst->AVCLevelIndication) {
		dst_tk = 0;
	} else if (avc_src->AVCProfileIndication!=avc_dst->AVCProfileIndication) {
		dst_tk = 0;
	}
	else {
		/*rewrite all samples if using different NALU size*/
		if (avc_src->nal_unit_size > avc_dst->nal_unit_size) {
			gf_media_avc_rewrite_samples(dest, dst_tk, 8*avc_dst->nal_unit_size, 8*avc_src->nal_unit_size);
			avc_dst->nal_unit_size = avc_src->nal_unit_size;
		} else if (avc_src->nal_unit_size < avc_dst->nal_unit_size) {
			gf_media_avc_rewrite_samples(orig, src_track, 8*avc_src->nal_unit_size, 8*avc_dst->nal_unit_size);
		}

		/*merge PS*/
		if (!merge_parameter_set(avc_src->sequenceParameterSets, avc_dst->sequenceParameterSets, "SPS"))
			dst_tk = 0;
		if (!merge_parameter_set(avc_src->pictureParameterSets, avc_dst->pictureParameterSets, "PPS"))
			dst_tk = 0;

		gf_isom_avc_config_update(dest, dst_tk, 1, avc_dst);
	}

	gf_odf_avc_cfg_del(avc_src);
	gf_odf_avc_cfg_del(avc_dst);

	if (!dst_tk) {
		dst_tk = gf_isom_get_track_by_id(dest, tk_id);
		gf_isom_set_nalu_extract_mode(orig, src_track, GF_ISOM_NALU_EXTRACT_INBAND_PS_FLAG);
		if (!force_cat) {
			gf_isom_avc_set_inband_config(dest, dst_tk, 1);
		} else {
			fprintf(stderr, "WARNING: Concatenating track ID %d even though sample descriptions do not match\n", tk_id);
		}
	}
	return dst_tk;
}
