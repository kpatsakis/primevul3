GF_Err pmax_dump(GF_Box *a, FILE * trace)
{
	GF_PMAXBox *p;
	p = (GF_PMAXBox *)a;
	gf_isom_box_dump_start(a, "MaxPacketSizeBox", trace);
	fprintf(trace, "MaximumSize=\"%d\">\n", p->maxSize);
	gf_isom_box_dump_done("MaxPacketSizeBox", a, trace);
	return GF_OK;
}
