static void tx3g_dump_box(FILE * trace, GF_BoxRecord *rec)
{
	fprintf(trace, "<BoxRecord top=\"%d\" left=\"%d\" bottom=\"%d\" right=\"%d\"/>\n", rec->top, rec->left, rec->bottom, rec->right);
}
