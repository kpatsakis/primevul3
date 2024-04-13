GF_Err trak_dump(GF_Box *a, FILE * trace)
{
	GF_TrackBox *p;

	p = (GF_TrackBox *)a;
	gf_isom_box_dump_start(a, "TrackBox", trace);
	fprintf(trace, ">\n");
	if (p->Header) {
		gf_isom_box_dump(p->Header, trace);
	} else if (p->size) {
		fprintf(trace, "<!--INVALID FILE: Missing Track Header-->\n");
	}
	if (p->References) gf_isom_box_dump(p->References, trace);
	if (p->meta) gf_isom_box_dump(p->meta, trace);
	if (p->editBox) gf_isom_box_dump(p->editBox, trace);
	if (p->Media) gf_isom_box_dump(p->Media, trace);
	if (p->groups) gf_isom_box_dump(p->groups, trace);
	if (p->udta) gf_isom_box_dump(p->udta, trace);
	gf_isom_box_dump_done("TrackBox", a, trace);
	return GF_OK;
}
