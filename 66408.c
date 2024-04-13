GF_Err pitm_dump(GF_Box *a, FILE * trace)
{
	GF_PrimaryItemBox *p = (GF_PrimaryItemBox *)a;
	gf_isom_box_dump_start(a, "PrimaryItemBox", trace);
	fprintf(trace, "item_ID=\"%d\">\n", p->item_ID);
	gf_isom_box_dump_done("PrimaryItemBox", a, trace);
	return GF_OK;
}
