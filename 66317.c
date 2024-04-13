GF_Err edts_dump(GF_Box *a, FILE * trace)
{
	GF_EditBox *p;

	p = (GF_EditBox *)a;
	gf_isom_box_dump_start(a, "EditBox", trace);
	fprintf(trace, ">\n");
	if (p->size)
		gf_isom_box_dump_ex(p->editList, trace, GF_ISOM_BOX_TYPE_ELST);
	gf_isom_box_dump_done("EditBox", a, trace);
	return GF_OK;
}
