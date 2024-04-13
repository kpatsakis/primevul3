GF_Err payt_dump(GF_Box *a, FILE * trace)
{
	GF_PAYTBox *p;
	p = (GF_PAYTBox *)a;
	gf_isom_box_dump_start(a, "PayloadTypeBox", trace);
	fprintf(trace, "PayloadID=\"%d\" PayloadString=\"%s\">\n", p->payloadCode, p->payloadString);
	gf_isom_box_dump_done("PayloadTypeBox", a, trace);
	return GF_OK;
}
