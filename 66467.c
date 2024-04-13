GF_Err trpy_dump(GF_Box *a, FILE * trace)
{
	GF_TRPYBox *p = (GF_TRPYBox *)a;
	gf_isom_box_dump_start(a, "LargeTotalRTPBytesBox", trace);
	fprintf(trace, "RTPBytesSent=\""LLD"\">\n", LLD_CAST p->nbBytes);
	gf_isom_box_dump_done("LargeTotalRTPBytesBox", a, trace);
	return GF_OK;
}
