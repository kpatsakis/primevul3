void list_modules(GF_ModuleManager *modules)
{
	u32 i;
	fprintf(stderr, "\rAvailable modules:\n");
	for (i=0; i<gf_modules_get_count(modules); i++) {
		char *str = (char *) gf_modules_get_file_name(modules, i);
		if (str) fprintf(stderr, "\t%s\n", str);
	}
	fprintf(stderr, "\n");
}
