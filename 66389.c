GF_Err moov_dump(GF_Box *a, FILE * trace)
{
	GF_MovieBox *p;
	p = (GF_MovieBox *)a;
	gf_isom_box_dump_start(a, "MovieBox", trace);
	fprintf(trace, ">\n");
	if (p->iods) gf_isom_box_dump(p->iods, trace);
	if (p->meta) gf_isom_box_dump(p->meta, trace);
	if (p->size)
		gf_isom_box_dump_ex(p->mvhd, trace,GF_ISOM_BOX_TYPE_MVHD);

#ifndef	GPAC_DISABLE_ISOM_FRAGMENTS
	if (p->mvex) gf_isom_box_dump(p->mvex, trace);
#endif

	gf_isom_box_array_dump(p->trackList, trace);
	if (p->udta) gf_isom_box_dump(p->udta, trace);
	gf_isom_box_dump_done("MovieBox", a, trace);
	return GF_OK;
}
