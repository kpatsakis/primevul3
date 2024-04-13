GF_Err dlay_dump(GF_Box *a, FILE * trace)
{
	GF_TextScrollDelayBox*p = (GF_TextScrollDelayBox*)a;
	gf_isom_box_dump_start(a, "TextScrollDelayBox", trace);
	fprintf(trace, "scroll_delay=\"%d\">\n", p->scroll_delay);
	gf_isom_box_dump_done("TextScrollDelayBox", a, trace);
	return GF_OK;
}
