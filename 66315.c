static void dump_data_string(FILE *trace, char *data, u32 dataLength)
{
	u32 i;
	for (i=0; i<dataLength; i++) {
		switch ((unsigned char) data[i]) {
		case '\'':
			fprintf(trace, "&apos;");
			break;
		case '\"':
			fprintf(trace, "&quot;");
			break;
		case '&':
			fprintf(trace, "&amp;");
			break;
		case '>':
			fprintf(trace, "&gt;");
			break;
		case '<':
			fprintf(trace, "&lt;");
			break;
		default:
			fprintf(trace, "%c", (u8) data[i]);
			break;
		}
	}
}
