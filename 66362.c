GF_Err iods_dump(GF_Box *a, FILE * trace)
{
	GF_ObjectDescriptorBox *p;

	p = (GF_ObjectDescriptorBox *)a;
	gf_isom_box_dump_start(a, "ObjectDescriptorBox", trace);
	fprintf(trace, ">\n");

	if (p->descriptor) {
#ifndef GPAC_DISABLE_OD_DUMP
		gf_odf_dump_desc(p->descriptor, trace, 1, GF_TRUE);
#else
		fprintf(trace, "<!-- Object Descriptor Dumping disabled in this build of GPAC -->\n");
#endif
	} else if (p->size) {
		fprintf(trace, "<!--WARNING: Object Descriptor not present-->\n");
	}
	gf_isom_box_dump_done("ObjectDescriptorBox", a, trace);
	return GF_OK;
}
