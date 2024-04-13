GF_Err gitn_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GroupIdToNameBox *ptr = (GroupIdToNameBox *) a;
	gf_isom_box_dump_start(a, "GroupIdToNameBox", trace);

	fprintf(trace, ">\n");

	for (i=0; i<ptr->nb_entries; i++) {
		fprintf(trace, "<GroupIdToNameBoxEntry groupID=\"%d\" name=\"%s\"/>\n", ptr->entries[i].group_id, ptr->entries[i].name);
	}
	if (!ptr->size) {
		fprintf(trace, "<GroupIdToNameBoxEntryEntry groupID=\"\" name=\"\"/>\n");
	}

	gf_isom_box_dump_done("GroupIdToNameBox", a, trace);
	return GF_OK;
}
