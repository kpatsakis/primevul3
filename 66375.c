GF_Err lsrc_dump(GF_Box *a, FILE * trace)
{
	GF_LASERConfigurationBox *p = (GF_LASERConfigurationBox *)a;
	gf_isom_box_dump_start(a, "LASeRConfigurationBox", trace);
	dump_data_attribute(trace, "LASeRHeader", p->hdr, p->hdr_size);
	fprintf(trace, ">");
	gf_isom_box_dump_done("LASeRConfigurationBox", a, trace);
	return GF_OK;
}
