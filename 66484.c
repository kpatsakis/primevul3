GF_Err url_dump(GF_Box *a, FILE * trace)
{
	GF_DataEntryURLBox *p;

	p = (GF_DataEntryURLBox *)a;
	gf_isom_box_dump_start(a, "URLDataEntryBox", trace);
	if (p->location) {
		fprintf(trace, " URL=\"%s\">\n", p->location);
	} else {
		fprintf(trace, ">\n");
		if (p->size) {
			if (! (p->flags & 1) ) {
				fprintf(trace, "<!--ERROR: No location indicated-->\n");
			} else {
				fprintf(trace, "<!--Data is contained in the movie file-->\n");
			}
		}
	}
	gf_isom_box_dump_done("URLDataEntryBox", a, trace);
	return GF_OK;
}
