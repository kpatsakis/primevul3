GF_Err tfdt_dump(GF_Box *a, FILE * trace)
{
	GF_TFBaseMediaDecodeTimeBox *ptr = (GF_TFBaseMediaDecodeTimeBox*) a;
	if (!a) return GF_BAD_PARAM;
	gf_isom_box_dump_start(a, "TrackFragmentBaseMediaDecodeTimeBox", trace);

	fprintf(trace, "baseMediaDecodeTime=\""LLD"\">\n", ptr->baseMediaDecodeTime);
	gf_isom_box_dump_done("TrackFragmentBaseMediaDecodeTimeBox", a, trace);
	return GF_OK;
}
