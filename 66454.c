GF_Err tols_dump(GF_Box *a, FILE * trace)
{
	GF_TargetOLSPropertyBox *ptr = (GF_TargetOLSPropertyBox *)a;
	gf_isom_box_dump_start(a, "TargetOLSPropertyBox", trace);
	fprintf(trace, "target_ols_index=\"%d\">\n", ptr->target_ols_index);

	gf_isom_box_dump_done("TargetOLSPropertyBox", a, trace);
	return GF_OK;
}
