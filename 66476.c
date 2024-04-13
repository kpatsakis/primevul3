static void tx3g_dump_rgba8(FILE * trace, char *name, u32 col)
{
	fprintf(trace, "%s=\"%x %x %x %x\"", name, (col>>16)&0xFF, (col>>8)&0xFF, (col)&0xFF, (col>>24)&0xFF);
}
