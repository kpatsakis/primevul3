GF_Err tfxd_dump(GF_Box *a, FILE * trace)
{
	GF_MSSTimeExtBox *ptr = (GF_MSSTimeExtBox*)a;
	if (!a) return GF_BAD_PARAM;
	gf_isom_box_dump_start(a, "MSSTimeExtensionBox", trace);
	fprintf(trace, "AbsoluteTime=\""LLU"\" FragmentDuration=\""LLU"\">\n", ptr->absolute_time_in_track_timescale, ptr->fragment_duration_in_track_timescale);
	fprintf(trace, "<FullBoxInfo Version=\"%d\" Flags=\"%d\"/>\n", ptr->version, ptr->flags);
	gf_isom_box_dump_done("MSSTimeExtensionBox", a, trace);
	return GF_OK;
}
