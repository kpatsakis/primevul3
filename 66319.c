GF_Err elst_dump(GF_Box *a, FILE * trace)
{
	GF_EditListBox *p;
	u32 i;
	GF_EdtsEntry *t;

	p = (GF_EditListBox *)a;
	gf_isom_box_dump_start(a, "EditListBox", trace);
	fprintf(trace, "EntryCount=\"%d\">\n", gf_list_count(p->entryList));

	i=0;
	while ((t = (GF_EdtsEntry *)gf_list_enum(p->entryList, &i))) {
		fprintf(trace, "<EditListEntry Duration=\""LLD"\" MediaTime=\""LLD"\" MediaRate=\"%u\"/>\n", LLD_CAST t->segmentDuration, LLD_CAST t->mediaTime, t->mediaRate);
	}
	if (!p->size) {
		fprintf(trace, "<EditListEntry Duration=\"\" MediaTime=\"\" MediaRate=\"\"/>\n");
	}
	gf_isom_box_dump_done("EditListBox", a, trace);
	return GF_OK;
}
