GF_Err styl_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_TextStyleBox*p = (GF_TextStyleBox*)a;
	gf_isom_box_dump_start(a, "TextStyleBox", trace);
	fprintf(trace, ">\n");
	for (i=0; i<p->entry_count; i++) tx3g_dump_style(trace, &p->styles[i]);
	if (!p->size) {
		fprintf(trace, "<StyleRecord startChar=\"\" endChar=\"\" fontID=\"\" styles=\"Normal|Bold|Italic|Underlined\" fontSize=\"\" textColor=\"\" />\n");
	}
	gf_isom_box_dump_done("TextStyleBox", a, trace);
	return GF_OK;
}
