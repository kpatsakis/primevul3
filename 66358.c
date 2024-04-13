GF_Err iloc_dump(GF_Box *a, FILE * trace)
{
	u32 i, j, count, count2;
	GF_ItemLocationBox *p = (GF_ItemLocationBox*)a;
	gf_isom_box_dump_start(a, "ItemLocationBox", trace);
	fprintf(trace, "offset_size=\"%d\" length_size=\"%d\" base_offset_size=\"%d\" index_size=\"%d\">\n", p->offset_size, p->length_size, p->base_offset_size, p->index_size);
	count = gf_list_count(p->location_entries);
	for (i=0; i<count; i++) {
		GF_ItemLocationEntry *ie = (GF_ItemLocationEntry *)gf_list_get(p->location_entries, i);
		count2 = gf_list_count(ie->extent_entries);
		fprintf(trace, "<ItemLocationEntry item_ID=\"%d\" data_reference_index=\"%d\" base_offset=\""LLD"\" construction_method=\"%d\">\n", ie->item_ID, ie->data_reference_index, LLD_CAST ie->base_offset, ie->construction_method);
		for (j=0; j<count2; j++) {
			GF_ItemExtentEntry *iee = (GF_ItemExtentEntry *)gf_list_get(ie->extent_entries, j);
			fprintf(trace, "<ItemExtentEntry extent_offset=\""LLD"\" extent_length=\""LLD"\" extent_index=\""LLD"\" />\n", LLD_CAST iee->extent_offset, LLD_CAST iee->extent_length, LLD_CAST iee->extent_index);
		}
		fprintf(trace, "</ItemLocationEntry>\n");
	}
	if (!p->size) {
		fprintf(trace, "<ItemLocationEntry item_ID=\"\" data_reference_index=\"\" base_offset=\"\" construction_method=\"\">\n");
		fprintf(trace, "<ItemExtentEntry extent_offset=\"\" extent_length=\"\" extent_index=\"\" />\n");
		fprintf(trace, "</ItemLocationEntry>\n");
	}
	gf_isom_box_dump_done("ItemLocationBox", a, trace);
	return GF_OK;
}
