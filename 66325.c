GF_Err fiin_dump(GF_Box *a, FILE * trace)
{
	FDItemInformationBox *ptr = (FDItemInformationBox *) a;
	gf_isom_box_dump_start(a, "FDItemInformationBox", trace);

	fprintf(trace, ">\n");
	if (ptr->partition_entries)
		gf_isom_box_array_dump(ptr->partition_entries, trace);

	if (ptr->session_info)
		gf_isom_box_dump(ptr->session_info, trace);

	if (ptr->group_id_to_name)
		gf_isom_box_dump(ptr->group_id_to_name, trace);

	gf_isom_box_dump_done("FDItemInformationBox", a, trace);
	return GF_OK;
}
