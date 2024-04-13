GF_Err mp4s_dump(GF_Box *a, FILE * trace)
{
	GF_MPEGSampleEntryBox *p;

	p = (GF_MPEGSampleEntryBox *)a;
	gf_isom_box_dump_start(a, "MPEGSystemsSampleDescriptionBox", trace);
	fprintf(trace, "DataReferenceIndex=\"%d\">\n", p->dataReferenceIndex);
	if (p->esd) {
		gf_isom_box_dump(p->esd, trace);
	} else if (p->size) {
		fprintf(trace, "<!--INVALID MP4 FILE: ESDBox not present in MPEG Sample Description or corrupted-->\n");
	}
	if (a->type == GF_ISOM_BOX_TYPE_ENCS) {
		gf_isom_box_array_dump(p->protections, trace);
	}
	gf_isom_box_dump_done("MPEGSystemsSampleDescriptionBox", a, trace);
	return GF_OK;
}
