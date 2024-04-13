GF_Err gnrv_dump(GF_Box *a, FILE * trace)
{
	GF_GenericVisualSampleEntryBox *p = (GF_GenericVisualSampleEntryBox *)a;
	if (p->EntryType)
		a->type = p->EntryType;

	gf_isom_box_dump_start(a, "VisualSampleDescriptionBox", trace);
	fprintf(trace, "DataReferenceIndex=\"%d\" Version=\"%d\" Revision=\"%d\" Vendor=\"%d\" TemporalQuality=\"%d\" SpacialQuality=\"%d\" Width=\"%d\" Height=\"%d\" HorizontalResolution=\"%d\" VerticalResolution=\"%d\" CompressorName=\"%s\" BitDepth=\"%d\">\n",
	        p->dataReferenceIndex, p->version, p->revision, p->vendor, p->temporal_quality, p->spatial_quality, p->Width, p->Height, p->horiz_res, p->vert_res, p->compressor_name+1, p->bit_depth);
	a->type = GF_ISOM_BOX_TYPE_GNRV;
	gf_isom_box_dump_done("VisualSampleDescriptionBox", a, trace);
	return GF_OK;
}
