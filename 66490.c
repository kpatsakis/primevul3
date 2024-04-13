GF_Err xml_dump(GF_Box *a, FILE * trace)
{
	GF_XMLBox *p = (GF_XMLBox *)a;
	gf_isom_box_dump_start(a, "XMLBox", trace);
	fprintf(trace, ">\n");
	fprintf(trace, "<![CDATA[\n");
	if (p->xml)
		gf_fwrite(p->xml, strlen(p->xml), 1, trace);
	fprintf(trace, "]]>\n");
	gf_isom_box_dump_done("XMLBox", a, trace);
	return GF_OK;
}
