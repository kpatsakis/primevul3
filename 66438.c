GF_Err stsh_dump(GF_Box *a, FILE * trace)
{
	GF_ShadowSyncBox *p;
	u32 i;
	GF_StshEntry *t;

	p = (GF_ShadowSyncBox *)a;
	gf_isom_box_dump_start(a, "SyncShadowBox", trace);
	fprintf(trace, "EntryCount=\"%d\">\n", gf_list_count(p->entries));
	i=0;
	while ((t = (GF_StshEntry *)gf_list_enum(p->entries, &i))) {
		fprintf(trace, "<SyncShadowEntry ShadowedSample=\"%d\" SyncSample=\"%d\"/>\n", t->shadowedSampleNumber, t->syncSampleNumber);
	}
	if (!p->size) {
		fprintf(trace, "<SyncShadowEntry ShadowedSample=\"\" SyncSample=\"\"/>\n");
	}
	gf_isom_box_dump_done("SyncShadowBox", a, trace);
	return GF_OK;
}
