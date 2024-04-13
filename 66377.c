GF_Err mdat_dump(GF_Box *a, FILE * trace)
{
	GF_MediaDataBox *p;
	const char *name = (a->type==GF_ISOM_BOX_TYPE_IDAT ? "ItemDataBox" : "MediaDataBox");
	p = (GF_MediaDataBox *)a;
	gf_isom_box_dump_start(a, name, trace);
	fprintf(trace, "dataSize=\""LLD"\">\n", LLD_CAST p->dataSize);
	gf_isom_box_dump_done(name, a, trace);
	return GF_OK;
}
