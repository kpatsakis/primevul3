static void tx3g_dump_style_nobox(FILE * trace, GF_StyleRecord *rec, u32 *shift_offset, u32 so_count)
{
	fprintf(trace, "<Style ");
	if (rec->startCharOffset || rec->endCharOffset)
		tx3g_print_char_offsets(trace, rec->startCharOffset, rec->endCharOffset, shift_offset, so_count);

	fprintf(trace, "styles=\"");
	if (!rec->style_flags) {
		fprintf(trace, "Normal");
	} else {
		if (rec->style_flags & 1) fprintf(trace, "Bold ");
		if (rec->style_flags & 2) fprintf(trace, "Italic ");
		if (rec->style_flags & 4) fprintf(trace, "Underlined ");
	}
	fprintf(trace, "\" fontID=\"%d\" fontSize=\"%d\" ", rec->fontID, rec->font_size);
	tx3g_dump_rgba8(trace, "color", rec->text_color);
	fprintf(trace, "/>\n");
}
