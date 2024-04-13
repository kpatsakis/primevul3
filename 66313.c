static void dump_data_attribute(FILE *trace, char *name, char *data, u32 data_size)
{
	u32 i;
	if (!data || !data_size) {
		fprintf(trace, "%s=\"\"", name);
		return;
	}
	fprintf(trace, "%s=\"0x", name);
	for (i=0; i<data_size; i++) fprintf(trace, "%02X", (unsigned char) data[i]);
	fprintf(trace, "\" ");
}
