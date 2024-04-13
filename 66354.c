GF_Err hvcc_dump(GF_Box *a, FILE * trace)
{
	u32 i, count;
	const char *name = (a->type==GF_ISOM_BOX_TYPE_HVCC) ? "HEVC" : "L-HEVC";
	char boxname[256];
	GF_HEVCConfigurationBox *p = (GF_HEVCConfigurationBox *) a;

	sprintf(boxname, "%sConfigurationBox", name);
	gf_isom_box_dump_start(a, boxname, trace);
	fprintf(trace, ">\n");

	if (! p->config) {
		if (p->size) {
			fprintf(trace, "<!-- INVALID HEVC ENTRY: no HEVC/SHVC config record -->\n");
		} else {
			fprintf(trace, "<%sDecoderConfigurationRecord nal_unit_size=\"\" configurationVersion=\"\" ", name);
			if (a->type==GF_ISOM_BOX_TYPE_HVCC) {
				fprintf(trace, "profile_space=\"\" tier_flag=\"\" profile_idc=\"\" general_profile_compatibility_flags=\"\" progressive_source_flag=\"\" interlaced_source_flag=\"\" non_packed_constraint_flag=\"\" frame_only_constraint_flag=\"\" constraint_indicator_flags=\"\" level_idc=\"\" ");
			}
			fprintf(trace, "min_spatial_segmentation_idc=\"\" parallelismType=\"\" ");

			if (a->type==GF_ISOM_BOX_TYPE_HVCC)
				fprintf(trace, "chroma_format=\"\" luma_bit_depth=\"\" chroma_bit_depth=\"\" avgFrameRate=\"\" constantFrameRate=\"\" numTemporalLayers=\"\" temporalIdNested=\"\"");

			fprintf(trace, ">\n");
			fprintf(trace, "<ParameterSetArray nalu_type=\"\" complete_set=\"\">\n");
			fprintf(trace, "<ParameterSet size=\"\" content=\"\"/>\n");
			fprintf(trace, "</ParameterSetArray>\n");
			fprintf(trace, "</%sDecoderConfigurationRecord>\n", name);
		}
		fprintf(trace, "</%sConfigurationBox>\n", name);
		return GF_OK;
	}

	fprintf(trace, "<%sDecoderConfigurationRecord nal_unit_size=\"%d\" ", name, p->config->nal_unit_size);
	fprintf(trace, "configurationVersion=\"%u\" ", p->config->configurationVersion);
	if (a->type==GF_ISOM_BOX_TYPE_HVCC) {
		fprintf(trace, "profile_space=\"%u\" ", p->config->profile_space);
		fprintf(trace, "tier_flag=\"%u\" ", p->config->tier_flag);
		fprintf(trace, "profile_idc=\"%u\" ", p->config->profile_idc);
		fprintf(trace, "general_profile_compatibility_flags=\"%X\" ", p->config->general_profile_compatibility_flags);
		fprintf(trace, "progressive_source_flag=\"%u\" ", p->config->progressive_source_flag);
		fprintf(trace, "interlaced_source_flag=\"%u\" ", p->config->interlaced_source_flag);
		fprintf(trace, "non_packed_constraint_flag=\"%u\" ", p->config->non_packed_constraint_flag);
		fprintf(trace, "frame_only_constraint_flag=\"%u\" ", p->config->frame_only_constraint_flag);
		fprintf(trace, "constraint_indicator_flags=\""LLX"\" ", p->config->constraint_indicator_flags);
		fprintf(trace, "level_idc=\"%d\" ", p->config->level_idc);
	}
	fprintf(trace, "min_spatial_segmentation_idc=\"%u\" ", p->config->min_spatial_segmentation_idc);
	fprintf(trace, "parallelismType=\"%u\" ", p->config->parallelismType);

	if (a->type==GF_ISOM_BOX_TYPE_HVCC)
		fprintf(trace, "chroma_format=\"%s\" luma_bit_depth=\"%u\" chroma_bit_depth=\"%u\" avgFrameRate=\"%u\" constantFrameRate=\"%u\" numTemporalLayers=\"%u\" temporalIdNested=\"%u\"",
	        gf_avc_hevc_get_chroma_format_name(p->config->chromaFormat), p->config->luma_bit_depth, p->config->chroma_bit_depth, p->config->avgFrameRate, p->config->constantFrameRate, p->config->numTemporalLayers, p->config->temporalIdNested);

	fprintf(trace, ">\n");

	count = gf_list_count(p->config->param_array);
	for (i=0; i<count; i++) {
		u32 nalucount, j;
		GF_HEVCParamArray *ar = (GF_HEVCParamArray*)gf_list_get(p->config->param_array, i);
		fprintf(trace, "<ParameterSetArray nalu_type=\"%d\" complete_set=\"%d\">\n", ar->type, ar->array_completeness);
		nalucount = gf_list_count(ar->nalus);
		for (j=0; j<nalucount; j++) {
			GF_AVCConfigSlot *c = (GF_AVCConfigSlot *)gf_list_get(ar->nalus, j);
			fprintf(trace, "<ParameterSet size=\"%d\" content=\"", c->size);
			dump_data(trace, c->data, c->size);
			fprintf(trace, "\"/>\n");
		}
		fprintf(trace, "</ParameterSetArray>\n");
	}

	fprintf(trace, "</%sDecoderConfigurationRecord>\n", name);

	gf_isom_box_dump_done(boxname, a, trace);
	return GF_OK;
}
