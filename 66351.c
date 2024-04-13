GF_Err hmhd_dump(GF_Box *a, FILE * trace)
{
	GF_HintMediaHeaderBox *p;

	p = (GF_HintMediaHeaderBox *)a;

	gf_isom_box_dump_start(a, "HintMediaHeaderBox", trace);
	fprintf(trace, "MaximumPDUSize=\"%d\" ", p->maxPDUSize);
	fprintf(trace, "AveragePDUSize=\"%d\" ", p->avgPDUSize);
	fprintf(trace, "MaxBitRate=\"%d\" ", p->maxBitrate);
	fprintf(trace, "AverageBitRate=\"%d\">\n", p->avgBitrate);

	gf_isom_box_dump_done("HintMediaHeaderBox", a, trace);
	return GF_OK;
}
