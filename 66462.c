GF_Err trex_dump(GF_Box *a, FILE * trace)
{
	GF_TrackExtendsBox *p;
	p = (GF_TrackExtendsBox *)a;
	gf_isom_box_dump_start(a, "TrackExtendsBox", trace);
	fprintf(trace, "TrackID=\"%d\"", p->trackID);
	fprintf(trace, " SampleDescriptionIndex=\"%d\" SampleDuration=\"%d\" SampleSize=\"%d\"", p->def_sample_desc_index, p->def_sample_duration, p->def_sample_size);
	fprintf(trace, ">\n");
	sample_flags_dump("DefaultSampleFlags", p->def_sample_flags, trace);
	gf_isom_box_dump_done("TrackExtendsBox", a, trace);
	return GF_OK;
}
