GF_Err tssy_dump(GF_Box *a, FILE * trace)
{
	GF_TimeStampSynchronyBox *p = (GF_TimeStampSynchronyBox *)a;
	gf_isom_box_dump_start(a, "TimeStampSynchronyBox", trace);
	fprintf(trace, "timestamp_sync=\"%d\">\n", p->timestamp_sync);
	gf_isom_box_dump_done("TimeStampSynchronyBox", a, trace);
	return GF_OK;
}
