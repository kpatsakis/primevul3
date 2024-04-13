GF_Err ipma_dump(GF_Box *a, FILE * trace)
{
	u32 i, j;
	GF_ItemPropertyAssociationBox *ptr = (GF_ItemPropertyAssociationBox *)a;
	u32 entry_count = gf_list_count(ptr->entries);
	if (!a) return GF_BAD_PARAM;
	gf_isom_box_dump_start(a, "ItemPropertyAssociationBox", trace);
	fprintf(trace, "entry_count=\"%d\">\n", entry_count);
	for (i = 0; i < entry_count; i++) {
		GF_ItemPropertyAssociationEntry *entry = (GF_ItemPropertyAssociationEntry *)gf_list_get(ptr->entries, i);
		u32 association_count = gf_list_count(entry->essential);
		fprintf(trace, "<AssociationEntry item_ID=\"%d\" association_count=\"%d\">\n", entry->item_id, association_count);
		for (j = 0; j < association_count; j++) {
			Bool *ess = (Bool *)gf_list_get(entry->essential, j);
			u32 *prop_index = (u32 *)gf_list_get(entry->property_index, j);
			fprintf(trace, "<Property index=\"%d\" essential=\"%d\"/>\n", *prop_index, *ess);
		}
		fprintf(trace, "</AssociationEntry>\n");
	}
	if (!ptr->size) {
		fprintf(trace, "<AssociationEntry item_ID=\"\" association_count=\"\">\n");
		fprintf(trace, "<Property index=\"\" essential=\"\"/>\n");
		fprintf(trace, "</AssociationEntry>\n");
	}
	gf_isom_box_dump_done("ItemPropertyAssociationBox", a, trace);
	return GF_OK;
}
