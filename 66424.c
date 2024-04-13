GF_Err segr_dump(GF_Box *a, FILE * trace)
{
	u32 i, k;
	FDSessionGroupBox *ptr = (FDSessionGroupBox *) a;
	gf_isom_box_dump_start(a, "FDSessionGroupBox", trace);
	fprintf(trace, ">\n");

	for (i=0; i<ptr->num_session_groups; i++) {
		fprintf(trace, "<FDSessionGroupBoxEntry groupIDs=\"");
		for (k=0; k<ptr->session_groups[i].nb_groups; k++) {
			fprintf(trace, "%d ", ptr->session_groups[i].group_ids[k]);
		}
		fprintf(trace, "\" channels=\"");
		for (k=0; k<ptr->session_groups[i].nb_channels; k++) {
			fprintf(trace, "%d ", ptr->session_groups[i].channels[k]);
		}
		fprintf(trace, "\"/>\n");
	}
	if (!ptr->size) {
		fprintf(trace, "<FDSessionGroupBoxEntry groupIDs=\"\" channels=\"\"/>\n");
	}

	gf_isom_box_dump_done("FDSessionGroupBox", a, trace);
	return GF_OK;
}
