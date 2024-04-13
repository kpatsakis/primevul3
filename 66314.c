static void dump_data_hex(FILE *trace, char *data, u32 dataLength)
{
	u32 i;
	fprintf(trace, "0x");
	for (i=0; i<dataLength; i++) {
		fprintf(trace, "%02X", (unsigned char) data[i]);
	}
}
