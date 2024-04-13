GF_Err oinf_dump(GF_Box *a, FILE * trace)
{
	GF_OINFPropertyBox *ptr = (GF_OINFPropertyBox *)a;
	gf_isom_box_dump_start(a, "OperatingPointsInformationPropertyBox", trace);
	fprintf(trace, ">\n");

	oinf_entry_dump(ptr->oinf, trace);

	gf_isom_box_dump_done("OperatingPointsInformationPropertyBox", a, trace);
	return GF_OK;
}
