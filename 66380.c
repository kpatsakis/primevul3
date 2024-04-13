GF_Err mdri_dump(GF_Box *a, FILE * trace)
{
	gf_isom_box_dump_start(a, "OMADRMMutableInformationBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("OMADRMMutableInformationBox", a, trace);
	return GF_OK;
}
