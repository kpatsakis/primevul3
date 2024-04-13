GF_Err co64_dump(GF_Box *a, FILE * trace)
{
	GF_ChunkLargeOffsetBox *p;
	u32 i;

	p = (GF_ChunkLargeOffsetBox *)a;
	gf_isom_box_dump_start(a, "ChunkLargeOffsetBox", trace);
	fprintf(trace, "EntryCount=\"%d\">\n", p->nb_entries);

	if (!p->offsets && p->size) {
		fprintf(trace, "<!-- Warning: No Chunk Offsets indications/>\n");
	} else {
		for (i=0; i<p->nb_entries; i++)
			fprintf(trace, "<ChunkOffsetEntry offset=\""LLU"\"/>\n", LLU_CAST p->offsets[i]);
	}
	if (!p->size) {
		fprintf(trace, "<ChunkOffsetEntry offset=\"\"/>\n");
	}
	gf_isom_box_dump_done("ChunkLargeOffsetBox", a, trace);
	return GF_OK;
}
