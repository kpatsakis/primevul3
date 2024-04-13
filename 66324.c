GF_Err fecr_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	char *box_name;
	FECReservoirBox *ptr = (FECReservoirBox *) a;
	if (a->type==GF_ISOM_BOX_TYPE_FIRE) {
		box_name = "FILEReservoirBox";
	} else {
		box_name = "FECReservoirBox";
	}
	gf_isom_box_dump_start(a, box_name, trace);

	fprintf(trace, ">\n");

	for (i=0; i<ptr->nb_entries; i++) {
		fprintf(trace, "<%sEntry itemID=\"%d\" symbol_count=\"%d\"/>\n", box_name, ptr->entries[i].item_id, ptr->entries[i].symbol_count);
	}
	if (!ptr->size) {
		fprintf(trace, "<%sEntry itemID=\"\" symbol_count=\"\"/>\n", box_name);
	}
	gf_isom_box_dump_done(box_name, a, trace);
	return GF_OK;
}
