static void tx3g_dump_style(FILE * trace, GF_StyleRecord *rec)
{
	fprintf(trace, "<StyleRecord startChar=\"%d\" endChar=\"%d\" fontID=\"%d\" styles=\"", rec->startCharOffset, rec->endCharOffset, rec->fontID);
	if (!rec->style_flags) {
		fprintf(trace, "Normal");
	} else {
		if (rec->style_flags & 1) fprintf(trace, "Bold ");
		if (rec->style_flags & 2) fprintf(trace, "Italic ");
		if (rec->style_flags & 4) fprintf(trace, "Underlined ");
	}
	fprintf(trace, "\" fontSize=\"%d\" ", rec->font_size);
	tx3g_dump_rgba8(trace, "textColor", rec->text_color);
	fprintf(trace, "/>\n");
}
