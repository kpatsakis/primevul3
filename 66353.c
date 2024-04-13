GF_Err href_dump(GF_Box *a, FILE * trace)
{
	GF_TextHyperTextBox*p = (GF_TextHyperTextBox*)a;
	gf_isom_box_dump_start(a, "TextHyperTextBox", trace);
	fprintf(trace, "startcharoffset=\"%d\" endcharoffset=\"%d\" URL=\"%s\" altString=\"%s\">\n", p->startcharoffset, p->endcharoffset, p->URL ? p->URL : "NULL", p->URL_hint ? p->URL_hint : "NULL");
	gf_isom_box_dump_done("TextHyperTextBox", a, trace);
	return GF_OK;
}
