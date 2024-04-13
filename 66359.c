GF_Err ilst_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_Box *tag;
	GF_Err e;
	GF_ItemListBox *ptr;
	ptr = (GF_ItemListBox *)a;
	gf_isom_box_dump_start(a, "ItemListBox", trace);
	fprintf(trace, ">\n");

	i=0;
	while ( (tag = (GF_Box*)gf_list_enum(ptr->other_boxes, &i))) {
		e = ilst_item_dump(tag, trace);
		if(e) return e;
	}
	gf_isom_box_dump_done("ItemListBox", NULL, trace);
	return GF_OK;
}
