GF_Err trik_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_TrickPlayBox *p = (GF_TrickPlayBox *) a;

	gf_isom_box_dump_start(a, "TrickPlayBox", trace);

	fprintf(trace, ">\n");
	for (i=0; i<p->entry_count; i++) {
		fprintf(trace, "<TrickPlayBoxEntry pic_type=\"%d\" dependency_level=\"%d\"/>\n", p->entries[i].pic_type, p->entries[i].dependency_level);
	}
	if (!p->size)
		fprintf(trace, "<TrickPlayBoxEntry pic_type=\"\" dependency_level=\"\"/>\n");

	gf_isom_box_dump_done("TrickPlayBox", a, trace);
	return GF_OK;
}
