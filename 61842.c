static int jas_icctxtdesc_output(jas_iccattrval_t *attrval, jas_stream_t *out)
{
	jas_icctxtdesc_t *txtdesc = &attrval->data.txtdesc;
	if (jas_iccputuint32(out, txtdesc->asclen) ||
	  jas_stream_puts(out, txtdesc->ascdata) ||
	  jas_stream_putc(out, 0) == EOF ||
	  jas_iccputuint32(out, txtdesc->uclangcode) ||
	  jas_iccputuint32(out, txtdesc->uclen) ||
	  jas_stream_write(out, txtdesc->ucdata, txtdesc->uclen * 2) != JAS_CAST(int, txtdesc->uclen * 2) ||
	  jas_iccputuint16(out, txtdesc->sccode) ||
	  jas_stream_putc(out, txtdesc->maclen) == EOF)
		goto error;
	if (txtdesc->maclen > 0) {
		if (jas_stream_write(out, txtdesc->macdata, 67) != 67)
			goto error;
	} else {
		if (jas_stream_pad(out, 67, 0) != 67)
			goto error;
	}
	return 0;
error:
	return -1;
}
