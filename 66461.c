GF_Err trep_dump(GF_Box *a, FILE * trace)
{
	GF_TrackExtensionPropertiesBox *p = (GF_TrackExtensionPropertiesBox*)a;
	gf_isom_box_dump_start(a, "TrackExtensionPropertiesBox", trace);
	fprintf(trace, "TrackID=\"%d\">\n", p->trackID);
	gf_isom_box_dump_done("TrackExtensionPropertiesBox", a, trace);
	return GF_OK;
}
