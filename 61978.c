int jpc_encode(jas_image_t *image, jas_stream_t *out, char *optstr)
{
	jpc_enc_t *enc;
	jpc_enc_cp_t *cp;

	enc = 0;
	cp = 0;

	jpc_initluts();

	if (!(cp = cp_create(optstr, image))) {
		jas_eprintf("invalid JP encoder options\n");
		goto error;
	}

	if (!(enc = jpc_enc_create(cp, out, image))) {
		goto error;
	}
	cp = 0;

	/* Encode the main header. */
	if (jpc_enc_encodemainhdr(enc)) {
		goto error;
	}

	/* Encode the main body.  This constitutes most of the encoding work. */
	if (jpc_enc_encodemainbody(enc)) {
		goto error;
	}

	/* Write EOC marker segment. */
	if (!(enc->mrk = jpc_ms_create(JPC_MS_EOC))) {
		goto error;
	}
	if (jpc_putms(enc->out, enc->cstate, enc->mrk)) {
		jas_eprintf("cannot write EOI marker\n");
		goto error;
	}
	jpc_ms_destroy(enc->mrk);
	enc->mrk = 0;

	if (jas_stream_flush(enc->out)) {
		goto error;
	}

	jpc_enc_destroy(enc);

	return 0;

error:
	if (cp) {
		jpc_enc_cp_destroy(cp);
	}
	if (enc) {
		jpc_enc_destroy(enc);
	}
	return -1;
}
