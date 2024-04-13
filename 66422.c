GF_Err schi_dump(GF_Box *a, FILE * trace)
{
	GF_SchemeInformationBox *p;
	p = (GF_SchemeInformationBox *)a;
	gf_isom_box_dump_start(a, "SchemeInformationBox", trace);
	fprintf(trace, ">\n");
	if (p->ikms) gf_isom_box_dump(p->ikms, trace);
	if (p->isfm) gf_isom_box_dump(p->isfm, trace);
	if (p->islt) gf_isom_box_dump(p->islt, trace);
	if (p->odkm) gf_isom_box_dump(p->odkm, trace);
	if (p->tenc) gf_isom_box_dump(p->tenc, trace);
	if (p->adkm) gf_isom_box_dump(p->adkm, trace);
	gf_isom_box_dump_done("SchemeInformationBox", a, trace);
	return GF_OK;
}
