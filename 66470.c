GF_Err tsro_dump(GF_Box *a, FILE * trace)
{
	GF_TimeOffHintEntryBox *p;
	p = (GF_TimeOffHintEntryBox *)a;
	gf_isom_box_dump_start(a, "TimeStampOffsetBox", trace);
	fprintf(trace, "TimeStampOffset=\"%d\">\n", p->TimeOffset);
	gf_isom_box_dump_done("TimeStampOffsetBox", a, trace);
	return GF_OK;
}
