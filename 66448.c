GF_Err tfra_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_TrackFragmentRandomAccessBox *p = (GF_TrackFragmentRandomAccessBox *)a;
	gf_isom_box_dump_start(a, "TrackFragmentRandomAccessBox", trace);
	fprintf(trace, "TrackId=\"%u\" number_of_entries=\"%u\">\n", p->track_id, p->nb_entries);
	for (i=0; i<p->nb_entries; i++) {
		fprintf(trace, "<RandomAccessEntry time=\""LLU"\" moof_offset=\""LLU"\" traf=\"%u\" trun=\"%u\" sample=\"%u\"/>\n",
			p->entries[i].time, p->entries[i].moof_offset,
			p->entries[i].traf_number, p->entries[i].trun_number, p->entries[i].sample_number);
	}
	if (!p->size) {
		fprintf(trace, "<RandomAccessEntry time=\"\" moof_offset=\"\" traf=\"\" trun=\"\" sample=\"\"/>\n");
	}
	gf_isom_box_dump_done("TrackFragmentRandomAccessBox", a, trace);
	return GF_OK;
}
