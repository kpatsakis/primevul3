GF_Err clap_dump(GF_Box *a, FILE * trace)
{
	GF_CleanAppertureBox *ptr = (GF_CleanAppertureBox*)a;
	gf_isom_box_dump_start(a, "CleanAppertureBox", trace);
	fprintf(trace, "cleanApertureWidthN=\"%d\" cleanApertureWidthD=\"%d\" ", ptr->cleanApertureWidthN, ptr->cleanApertureWidthD);
	fprintf(trace, "cleanApertureHeightN=\"%d\" cleanApertureHeightD=\"%d\" ", ptr->cleanApertureHeightN, ptr->cleanApertureHeightD);
	fprintf(trace, "horizOffN=\"%d\" horizOffD=\"%d\" ", ptr->horizOffN, ptr->horizOffD);
	fprintf(trace, "vertOffN=\"%d\" vertOffD=\"%d\"", ptr->vertOffN, ptr->vertOffD);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("CleanAppertureBox", a, trace);
	return GF_OK;
}
