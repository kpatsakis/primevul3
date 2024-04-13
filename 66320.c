GF_Err esds_dump(GF_Box *a, FILE * trace)
{
	GF_ESDBox *p;

	p = (GF_ESDBox *)a;
	gf_isom_box_dump_start(a, "MPEG4ESDescriptorBox", trace);
	fprintf(trace, ">\n");

	if (p->desc) {
#ifndef GPAC_DISABLE_OD_DUMP
		gf_odf_dump_desc((GF_Descriptor *) p->desc, trace, 1, GF_TRUE);
#else
		fprintf(trace, "<!-- Object Descriptor Dumping disabled in this build of GPAC -->\n");
#endif
	} else if (p->size) {
		fprintf(trace, "<!--INVALID MP4 FILE: ESD not present in MPEG Sample Description or corrupted-->\n");
	}
	gf_isom_box_dump_done("MPEG4ESDescriptorBox", a, trace);
	return GF_OK;
}
