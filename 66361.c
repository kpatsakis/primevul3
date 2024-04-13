GF_Err infe_dump(GF_Box *a, FILE * trace)
{
	GF_ItemInfoEntryBox *p = (GF_ItemInfoEntryBox *)a;
	gf_isom_box_dump_start(a, "ItemInfoEntryBox", trace);
	fprintf(trace, "item_ID=\"%d\" item_protection_index=\"%d\" item_name=\"%s\" content_type=\"%s\" content_encoding=\"%s\" item_type=\"%s\">\n", p->item_ID, p->item_protection_index, p->item_name, p->content_type, p->content_encoding, gf_4cc_to_str(p->item_type));
	gf_isom_box_dump_done("ItemInfoEntryBox", a, trace);
	return GF_OK;
}
