GF_Err krok_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_TextKaraokeBox*p = (GF_TextKaraokeBox*)a;
	gf_isom_box_dump_start(a, "TextKaraokeBox", trace);
	fprintf(trace, "highlight_starttime=\"%d\">\n", p->highlight_starttime);
	for (i=0; i<p->nb_entries; i++) {
		fprintf(trace, "<KaraokeRecord highlight_endtime=\"%d\" start_charoffset=\"%d\" end_charoffset=\"%d\"/>\n", p->records[i].highlight_endtime, p->records[i].start_charoffset, p->records[i].end_charoffset);
	}
	if (!p->size) {
		fprintf(trace, "<KaraokeRecord highlight_endtime=\"\" start_charoffset=\"\" end_charoffset=\"\"/>\n");
	}
	gf_isom_box_dump_done("TextKaraokeBox", a, trace);
	return GF_OK;
}
