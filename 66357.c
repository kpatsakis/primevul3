GF_Err iinf_dump(GF_Box *a, FILE * trace)
{
	GF_ItemInfoBox *p = (GF_ItemInfoBox *)a;
	gf_isom_box_dump_start(a, "ItemInfoBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_array_dump(p->item_infos, trace);
	gf_isom_box_dump_done("ItemInfoBox", a, trace);
	return GF_OK;
}
