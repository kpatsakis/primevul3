GF_Err txtc_dump(GF_Box *a, FILE * trace)
{
	GF_TextConfigBox *ptr = (GF_TextConfigBox*)a;
	const char *name = "TextConfigBox";

	gf_isom_box_dump_start(a, name, trace);
	fprintf(trace, ">\n");

	if (ptr->config) fprintf(trace, "<![CDATA[%s]]>", ptr->config);

	gf_isom_box_dump_done(name, a, trace);
	return GF_OK;
}
