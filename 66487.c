GF_Err video_sample_entry_dump(GF_Box *a, FILE * trace)
{
	GF_MPEGVisualSampleEntryBox *p = (GF_MPEGVisualSampleEntryBox *)a;
	const char *name;

	switch (p->type) {
	case GF_ISOM_SUBTYPE_AVC_H264:
	case GF_ISOM_SUBTYPE_AVC2_H264:
	case GF_ISOM_SUBTYPE_AVC3_H264:
	case GF_ISOM_SUBTYPE_AVC4_H264:
		name = "AVCSampleEntryBox";
		break;
	case GF_ISOM_SUBTYPE_MVC_H264:
		name = "MVCSampleEntryBox";
		break;
	case GF_ISOM_SUBTYPE_SVC_H264:
		name = "SVCSampleEntryBox";
		break;
	case GF_ISOM_SUBTYPE_HVC1:
	case GF_ISOM_SUBTYPE_HEV1:
	case GF_ISOM_SUBTYPE_HVC2:
	case GF_ISOM_SUBTYPE_HEV2:
		name = "HEVCSampleEntryBox";
		break;
	case GF_ISOM_SUBTYPE_LHV1:
	case GF_ISOM_SUBTYPE_LHE1:
		name = "LHEVCSampleEntryBox";
		break;
	case GF_ISOM_SUBTYPE_3GP_H263:
		name = "H263SampleDescriptionBox";
		break;
	default:
		name = "MPEGVisualSampleDescriptionBox";
	}

	gf_isom_box_dump_start(a, name, trace);

	fprintf(trace, " DataReferenceIndex=\"%d\" Width=\"%d\" Height=\"%d\"", p->dataReferenceIndex, p->Width, p->Height);

	fprintf(trace, " XDPI=\"%d\" YDPI=\"%d\" BitDepth=\"%d\"", p->horiz_res, p->vert_res, p->bit_depth);
	if (strlen((const char*)p->compressor_name) )
		fprintf(trace, " CompressorName=\"%s\"\n", p->compressor_name+1);


	fprintf(trace, ">\n");

	if (p->esd) {
		gf_isom_box_dump(p->esd, trace);
	} else {
		if (p->hevc_config) gf_isom_box_dump(p->hevc_config, trace);
		if (p->avc_config) gf_isom_box_dump(p->avc_config, trace);
		if (p->ipod_ext) gf_isom_box_dump(p->ipod_ext, trace);
		if (p->descr) gf_isom_box_dump(p->descr, trace);
		if (p->svc_config) gf_isom_box_dump(p->svc_config, trace);
		if (p->mvc_config) gf_isom_box_dump(p->mvc_config, trace);
		if (p->lhvc_config) gf_isom_box_dump(p->lhvc_config, trace);
		if (p->cfg_3gpp) gf_isom_box_dump(p->cfg_3gpp, trace);
	}
	if (a->type == GF_ISOM_BOX_TYPE_ENCV) {
		gf_isom_box_array_dump(p->protections, trace);
	}
	if (p->pasp) gf_isom_box_dump(p->pasp, trace);
	if (p->rvcc) gf_isom_box_dump(p->rvcc, trace);
	if (p->rinf) gf_isom_box_dump(p->rinf, trace);

	gf_isom_box_dump_done(name, a, trace);
	return GF_OK;
}
