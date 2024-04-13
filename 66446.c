GF_Err text_dump(GF_Box *a, FILE * trace)
{
	GF_TextSampleEntryBox *p = (GF_TextSampleEntryBox *)a;
	gf_isom_box_dump_start(a, "TextSampleEntryBox", trace);
	fprintf(trace, "dataReferenceIndex=\"%d\" displayFlags=\"%x\" textJustification=\"%d\"  ",
	        p->dataReferenceIndex, p->displayFlags, p->textJustification);
	if (p->textName)
		fprintf(trace, "textName=\"%s\" ", p->textName);
	tx3g_dump_rgb16(trace, "background-color", p->background_color);
	tx3g_dump_rgb16(trace, " foreground-color", p->foreground_color);
	fprintf(trace, ">\n");

	fprintf(trace, "<DefaultBox>\n");
	tx3g_dump_box(trace, &p->default_box);
	gf_isom_box_dump_done("DefaultBox", a, trace);
	gf_isom_box_dump_done("TextSampleEntryBox", a, trace);
	return GF_OK;
}
