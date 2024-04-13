GF_Err trun_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_TrunEntry *ent;
	GF_TrackFragmentRunBox *p;

	p = (GF_TrackFragmentRunBox *)a;
	gf_isom_box_dump_start(a, "TrackRunBox", trace);
	fprintf(trace, "SampleCount=\"%d\"", p->sample_count);

	if (p->flags & GF_ISOM_TRUN_DATA_OFFSET)
		fprintf(trace, " DataOffset=\"%d\"", p->data_offset);
	fprintf(trace, ">\n");

	if (p->flags & GF_ISOM_TRUN_FIRST_FLAG) {
		sample_flags_dump("FirstSampleFlags", p->first_sample_flags, trace);
	}

	if (p->flags & (GF_ISOM_TRUN_DURATION|GF_ISOM_TRUN_SIZE|GF_ISOM_TRUN_CTS_OFFSET|GF_ISOM_TRUN_FLAGS)) {
		i=0;
		while ((ent = (GF_TrunEntry *)gf_list_enum(p->entries, &i))) {

			fprintf(trace, "<TrackRunEntry");

			if (p->flags & GF_ISOM_TRUN_DURATION)
				fprintf(trace, " Duration=\"%u\"", ent->Duration);
			if (p->flags & GF_ISOM_TRUN_SIZE)
				fprintf(trace, " Size=\"%u\"", ent->size);
			if (p->flags & GF_ISOM_TRUN_CTS_OFFSET)
			{
				if (p->version == 0)
					fprintf(trace, " CTSOffset=\"%u\"", (u32) ent->CTS_Offset);
				else
					fprintf(trace, " CTSOffset=\"%d\"", ent->CTS_Offset);
			}

			if (p->flags & GF_ISOM_TRUN_FLAGS) {
				frag_dump_sample_flags(trace, ent->flags);
			}
			fprintf(trace, "/>\n");
		}
	} else if (p->size) {
		fprintf(trace, "<!-- all default values used -->\n");
	} else {
		fprintf(trace, "<TrackRunEntry Duration=\"\" Size=\"\" CTSOffset=\"\"");
		frag_dump_sample_flags(trace, 0);
		fprintf(trace, "/>\n");
	}

	gf_isom_box_dump_done("TrackRunBox", a, trace);
	return GF_OK;
}
