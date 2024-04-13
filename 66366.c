GF_Err iref_dump(GF_Box *a, FILE * trace)
{
	GF_ItemReferenceBox *p = (GF_ItemReferenceBox *)a;
	gf_isom_box_dump_start(a, "ItemReferenceBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_array_dump(p->references, trace);
	gf_isom_box_dump_done("ItemReferenceBox", a, trace);
	return GF_OK;
}
