GF_Err bxml_dump(GF_Box *a, FILE * trace)
{
	GF_BinaryXMLBox *p = (GF_BinaryXMLBox *)a;
	gf_isom_box_dump_start(a, "BinaryXMLBox", trace);
	fprintf(trace, "binarySize=\"%d\">\n", p->data_length);
	gf_isom_box_dump_done("BinaryXMLBox", a, trace);
	return GF_OK;
}
