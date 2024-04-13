GF_Err btrt_dump(GF_Box *a, FILE * trace)
{
	GF_BitRateBox *p = (GF_BitRateBox*)a;
	gf_isom_box_dump_start(a, "BitRateBox", trace);
	fprintf(trace, "BufferSizeDB=\"%d\" avgBitRate=\"%d\" maxBitRate=\"%d\">\n", p->bufferSizeDB, p->avgBitrate, p->maxBitrate);
	gf_isom_box_dump_done("BitRateBox", a, trace);
	return GF_OK;
}
