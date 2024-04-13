GF_Err free_dump(GF_Box *a, FILE * trace)
{
	GF_FreeSpaceBox *p = (GF_FreeSpaceBox *)a;
	gf_isom_box_dump_start(a, (a->type==GF_ISOM_BOX_TYPE_FREE) ? "FreeSpaceBox" : "SkipBox", trace);
	fprintf(trace, "dataSize=\"%d\">\n", p->dataSize);
	gf_isom_box_dump_done( (a->type==GF_ISOM_BOX_TYPE_FREE) ? "FreeSpaceBox" : "SkipBox", a, trace);
	return GF_OK;
}
