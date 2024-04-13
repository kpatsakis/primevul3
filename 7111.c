static void cfg_indent(FILE *fp, int indent)
{
	while (indent--)
		fprintf(fp, "  ");
}