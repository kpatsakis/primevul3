GF_Err paen_dump(GF_Box *a, FILE * trace)
{
	FDPartitionEntryBox *ptr = (FDPartitionEntryBox *) a;
	gf_isom_box_dump_start(a, "FDPartitionEntryBox", trace);

	fprintf(trace, ">\n");
	if (ptr->blocks_and_symbols)
		gf_isom_box_dump(ptr->blocks_and_symbols, trace);

	if (ptr->FEC_symbol_locations)
		gf_isom_box_dump(ptr->FEC_symbol_locations, trace);

	if (ptr->FEC_symbol_locations)
		gf_isom_box_dump(ptr->FEC_symbol_locations, trace);

	gf_isom_box_dump_done("FDPartitionEntryBox", a, trace);
	return GF_OK;
}
