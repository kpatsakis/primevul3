GF_Err stsf_dump(GF_Box *a, FILE * trace)
{
	GF_SampleFragmentBox *p;
	GF_StsfEntry *ent;
	u32 i, j, count;


	p = (GF_SampleFragmentBox *)a;
	count = gf_list_count(p->entryList);
	gf_isom_box_dump_start(a, "SampleFragmentBox", trace);
	fprintf(trace, "EntryCount=\"%d\">\n", count);

	for (i=0; i<count; i++) {
		ent = (GF_StsfEntry *)gf_list_get(p->entryList, i);
		fprintf(trace, "<SampleFragmentEntry SampleNumber=\"%d\" FragmentCount=\"%d\">\n", ent->SampleNumber, ent->fragmentCount);
		for (j=0; j<ent->fragmentCount; j++) fprintf(trace, "<FragmentSizeEntry size=\"%d\"/>\n", ent->fragmentSizes[j]);
		fprintf(trace, "</SampleFragmentEntry>\n");
	}
	if (!p->size) {
		fprintf(trace, "<SampleFragmentEntry SampleNumber=\"\" FragmentCount=\"\">\n");
		fprintf(trace, "<FragmentSizeEntry size=\"\"/>\n");
		fprintf(trace, "</SampleFragmentEntry>\n");
	}
	gf_isom_box_dump_done("SampleFragmentBox", a, trace);
	return GF_OK;
}
