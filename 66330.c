GF_Err ftab_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_FontTableBox *p = (GF_FontTableBox *)a;
	gf_isom_box_dump_start(a, "FontTableBox", trace);
	fprintf(trace, ">\n");
	for (i=0; i<p->entry_count; i++) {
		fprintf(trace, "<FontRecord ID=\"%d\" name=\"%s\"/>\n", p->fonts[i].fontID, p->fonts[i].fontName ? p->fonts[i].fontName : "NULL");
	}
	if (!p->size) {
		fprintf(trace, "<FontRecord ID=\"\" name=\"\"/>\n");
	}
	gf_isom_box_dump_done("FontTableBox", a, trace);
	return GF_OK;
}
