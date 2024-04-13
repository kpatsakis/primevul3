static void dump_escape_string(FILE * trace, char *name)
{
	u32 i, len = (u32) strlen(name);
	for (i=0; i<len; i++) {
		if (name[i]=='"') fprintf(trace, "&quot;");
		else fputc(name[i], trace);
	}
}
