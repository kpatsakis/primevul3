static int jas_icctxt_output(jas_iccattrval_t *attrval, jas_stream_t *out)
{
	jas_icctxt_t *txt = &attrval->data.txt;
	if (jas_stream_puts(out, txt->string) ||
	  jas_stream_putc(out, 0) == EOF)
		return -1;
	return 0;
}
