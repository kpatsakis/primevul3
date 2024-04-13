GF_Err ireftype_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_ItemReferenceTypeBox *p = (GF_ItemReferenceTypeBox *)a;
	if (!p->reference_type) return GF_OK;

	p->type = p->reference_type;
	gf_isom_box_dump_start(a, "ItemReferenceBox", trace);
	fprintf(trace, "from_item_id=\"%d\">\n", p->from_item_id);
	for (i = 0; i < p->reference_count; i++) {
		fprintf(trace, "<ItemReferenceBoxEntry ItemID=\"%d\"/>\n", p->to_item_IDs[i]);
	}
	if (!p->size)
		fprintf(trace, "<ItemReferenceBoxEntry ItemID=\"\"/>\n");

	gf_isom_box_dump_done("ItemReferenceBox", a, trace);

	p->type = GF_ISOM_BOX_TYPE_REFI;
	return GF_OK;
}
