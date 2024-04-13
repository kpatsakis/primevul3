GF_Err mdia_dump(GF_Box *a, FILE * trace)
{
	GF_MediaBox *p = (GF_MediaBox *)a;
	gf_isom_box_dump_start(a, "MediaBox", trace);
	fprintf(trace, ">\n");
	if (p->size)
		gf_isom_box_dump_ex(p->mediaHeader, trace, GF_ISOM_BOX_TYPE_MDHD);
	if (p->size)
		gf_isom_box_dump_ex(p->handler, trace,GF_ISOM_BOX_TYPE_HDLR);
	if (p->size)
		gf_isom_box_dump_ex(p->information, trace, GF_ISOM_BOX_TYPE_MINF);
	gf_isom_box_dump_done("MediaBox", a, trace);
	return GF_OK;
}
