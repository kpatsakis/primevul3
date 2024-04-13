GF_Err hlit_dump(GF_Box *a, FILE * trace)
{
	GF_TextHighlightBox*p = (GF_TextHighlightBox*)a;
	gf_isom_box_dump_start(a, "TextHighlightBox", trace);
	fprintf(trace, "startcharoffset=\"%d\" endcharoffset=\"%d\">\n", p->startcharoffset, p->endcharoffset);
	gf_isom_box_dump_done("TextHighlightBox", a, trace);
	return GF_OK;
}
