static int jas_icclut16_output(jas_iccattrval_t *attrval, jas_stream_t *out)
{
	jas_icclut16_t *lut16 = &attrval->data.lut16;
	int i;
	int j;
	int n;
	if (jas_stream_putc(out, lut16->numinchans) == EOF ||
	  jas_stream_putc(out, lut16->numoutchans) == EOF ||
	  jas_stream_putc(out, lut16->clutlen) == EOF ||
	  jas_stream_putc(out, 0) == EOF)
		goto error;
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 3; ++j) {
			if (jas_iccputsint32(out, lut16->e[i][j]))
				goto error;
		}
	}
	if (jas_iccputuint16(out, lut16->numintabents) ||
	  jas_iccputuint16(out, lut16->numouttabents))
		goto error;
	n = lut16->numinchans * lut16->numintabents;
	for (i = 0; i < n; ++i) {
		if (jas_iccputuint16(out, lut16->intabsbuf[i]))
			goto error;
	}
	n = lut16->numoutchans * lut16->numouttabents;
	for (i = 0; i < n; ++i) {
		if (jas_iccputuint16(out, lut16->outtabsbuf[i]))
			goto error;
	}
	n = jas_iccpowi(lut16->clutlen, lut16->numinchans) * lut16->numoutchans;
	for (i = 0; i < n; ++i) {
		if (jas_iccputuint16(out, lut16->clut[i]))
			goto error;
	}
	return 0;
error:
	return -1;
}
