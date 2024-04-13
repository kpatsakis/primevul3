jpc_enc_t *jpc_enc_create(jpc_enc_cp_t *cp, jas_stream_t *out, jas_image_t *image)
{
	jpc_enc_t *enc;

	enc = 0;

	if (!(enc = jas_malloc(sizeof(jpc_enc_t)))) {
		goto error;
	}

	enc->image = image;
	enc->out = out;
	enc->cp = cp;
	enc->cstate = 0;
	enc->tmpstream = 0;
	enc->mrk = 0;
	enc->curtile = 0;

	if (!(enc->cstate = jpc_cstate_create())) {
		goto error;
	}
	enc->len = 0;
	enc->mainbodysize = 0;

	return enc;

error:

	if (enc) {
		jpc_enc_destroy(enc);
	}
	return 0;
}
