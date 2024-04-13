void PrintWorldInfo(GF_Terminal *term)
{
	u32 i;
	const char *title;
	GF_List *descs;
	descs = gf_list_new();
	title = gf_term_get_world_info(term, NULL, descs);
	if (!title && !gf_list_count(descs)) {
		fprintf(stderr, "No World Info available\n");
	} else {
		fprintf(stderr, "\t%s\n", title ? title : "No title available");
		for (i=0; i<gf_list_count(descs); i++) {
			char *str = gf_list_get(descs, i);
			fprintf(stderr, "%s\n", str);
		}
	}
	gf_list_del(descs);
}
