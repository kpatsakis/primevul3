static int mif_hdr_put(mif_hdr_t *hdr, jas_stream_t *out)
{
	int cmptno;
	mif_cmpt_t *cmpt;

	/* Output signature. */
	jas_stream_putc(out, (MIF_MAGIC >> 24) & 0xff);
	jas_stream_putc(out, (MIF_MAGIC >> 16) & 0xff);
	jas_stream_putc(out, (MIF_MAGIC >> 8) & 0xff);
	jas_stream_putc(out, MIF_MAGIC & 0xff);

	/* Output component information. */
	for (cmptno = 0; cmptno < hdr->numcmpts; ++cmptno) {
		cmpt = hdr->cmpts[cmptno];
		jas_stream_printf(out, "component tlx=%ld tly=%ld "
		  "sampperx=%ld samppery=%ld width=%ld height=%ld prec=%d sgnd=%d",
		  cmpt->tlx, cmpt->tly, cmpt->sampperx, cmpt->samppery, cmpt->width,
		  cmpt->height, cmpt->prec, cmpt->sgnd);
		if (cmpt->data) {
			jas_stream_printf(out, " data=%s", cmpt->data);
		}
		jas_stream_printf(out, "\n");
	}

	/* Output end of header indicator. */
	jas_stream_printf(out, "end\n");

	return 0;
}
