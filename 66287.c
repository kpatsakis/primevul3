GF_Err avcc_dump(GF_Box *a, FILE * trace)
{
	u32 i, count;
	GF_AVCConfigurationBox *p = (GF_AVCConfigurationBox *) a;
	const char *name = (p->type==GF_ISOM_BOX_TYPE_MVCC) ? "MVC" : (p->type==GF_ISOM_BOX_TYPE_SVCC) ? "SVC" : "AVC";
	char boxname[256];
	sprintf(boxname, "%sConfigurationBox", name);
	gf_isom_box_dump_start(a, boxname, trace);
	fprintf(trace, ">\n");

	fprintf(trace, "<%sDecoderConfigurationRecord", name);

	if (! p->config) {
		if (p->size) {
			fprintf(trace, ">\n");
			fprintf(trace, "<!-- INVALID AVC ENTRY : no AVC/SVC config record -->\n");
		} else {

			fprintf(trace, " configurationVersion=\"\" AVCProfileIndication=\"\" profile_compatibility=\"\" AVCLevelIndication=\"\" nal_unit_size=\"\" complete_representation=\"\"");

			fprintf(trace, " chroma_format=\"\" luma_bit_depth=\"\" chroma_bit_depth=\"\"");
			fprintf(trace, ">\n");

			fprintf(trace, "<SequenceParameterSet size=\"\" content=\"\"/>\n");
			fprintf(trace, "<PictureParameterSet size=\"\" content=\"\"/>\n");
			fprintf(trace, "<SequenceParameterSetExtensions size=\"\" content=\"\"/>\n");
		}
		fprintf(trace, "</%sDecoderConfigurationRecord>\n", name);
		gf_isom_box_dump_done(boxname, a, trace);
		return GF_OK;
	}

	fprintf(trace, " configurationVersion=\"%d\" AVCProfileIndication=\"%d\" profile_compatibility=\"%d\" AVCLevelIndication=\"%d\" nal_unit_size=\"%d\"", p->config->configurationVersion, p->config->AVCProfileIndication, p->config->profile_compatibility, p->config->AVCLevelIndication, p->config->nal_unit_size);


	if ((p->type==GF_ISOM_BOX_TYPE_SVCC) || (p->type==GF_ISOM_BOX_TYPE_MVCC) )
		fprintf(trace, " complete_representation=\"%d\"", p->config->complete_representation);

	if (p->type==GF_ISOM_BOX_TYPE_AVCC) {
		if (gf_avc_is_rext_profile(p->config->AVCProfileIndication)) {
			fprintf(trace, " chroma_format=\"%s\" luma_bit_depth=\"%d\" chroma_bit_depth=\"%d\"", gf_avc_hevc_get_chroma_format_name(p->config->chroma_format), p->config->luma_bit_depth, p->config->chroma_bit_depth);
		}
	}

	fprintf(trace, ">\n");

	count = gf_list_count(p->config->sequenceParameterSets);
	for (i=0; i<count; i++) {
		GF_AVCConfigSlot *c = (GF_AVCConfigSlot *)gf_list_get(p->config->sequenceParameterSets, i);
		fprintf(trace, "<SequenceParameterSet size=\"%d\" content=\"", c->size);
		dump_data(trace, c->data, c->size);
		fprintf(trace, "\"/>\n");
	}
	count = gf_list_count(p->config->pictureParameterSets);
	for (i=0; i<count; i++) {
		GF_AVCConfigSlot *c = (GF_AVCConfigSlot *)gf_list_get(p->config->pictureParameterSets, i);
		fprintf(trace, "<PictureParameterSet size=\"%d\" content=\"", c->size);
		dump_data(trace, c->data, c->size);
		fprintf(trace, "\"/>\n");
	}

	if (p->config->sequenceParameterSetExtensions) {
		count = gf_list_count(p->config->sequenceParameterSetExtensions);
		for (i=0; i<count; i++) {
			GF_AVCConfigSlot *c = (GF_AVCConfigSlot *)gf_list_get(p->config->sequenceParameterSetExtensions, i);
			fprintf(trace, "<SequenceParameterSetExtensions size=\"%d\" content=\"", c->size);
			dump_data(trace, c->data, c->size);
			fprintf(trace, "\"/>\n");
		}
	}

	fprintf(trace, "</%sDecoderConfigurationRecord>\n", name);

	gf_isom_box_dump_done(boxname, a, trace);
	return GF_OK;
}
