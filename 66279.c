GF_Err afra_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_AdobeFragRandomAccessBox *p = (GF_AdobeFragRandomAccessBox*)a;
	gf_isom_box_dump_start(a, "AdobeFragmentRandomAccessBox", trace);

	fprintf(trace, "LongIDs=\"%u\" LongOffsets=\"%u\" TimeScale=\"%u\">\n", p->long_ids, p->long_offsets, p->time_scale);

	for (i=0; i<p->entry_count; i++) {
		GF_AfraEntry *ae = (GF_AfraEntry *)gf_list_get(p->local_access_entries, i);
		fprintf(trace, "<LocalAccessEntry Time=\""LLU"\" Offset=\""LLU"\"/>\n", ae->time, ae->offset);
	}

	for (i=0; i<p->global_entry_count; i++) {
		GF_GlobalAfraEntry *gae = (GF_GlobalAfraEntry *)gf_list_get(p->global_access_entries, i);
		fprintf(trace, "<GlobalAccessEntry Time=\""LLU"\" Segment=\"%u\" Fragment=\"%u\" AfraOffset=\""LLU"\" OffsetFromAfra=\""LLU"\"/>\n",
		        gae->time, gae->segment, gae->fragment, gae->afra_offset, gae->offset_from_afra);
	}

	gf_isom_box_dump_done("AdobeFragmentRandomAccessBox", a, trace);
	return GF_OK;
}
