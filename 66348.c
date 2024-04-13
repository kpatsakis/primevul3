GF_Err hclr_dump(GF_Box *a, FILE * trace)
{
	GF_TextHighlightColorBox*p = (GF_TextHighlightColorBox*)a;
	gf_isom_box_dump_start(a, "TextHighlightColorBox", trace);
	tx3g_dump_rgba8(trace, "highlight_color", p->hil_color);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("TextHighlightColorBox", a, trace);
	return GF_OK;
}
