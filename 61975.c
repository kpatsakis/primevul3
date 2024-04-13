int jpc_enc_encodetiledata(jpc_enc_t *enc)
{
assert(enc->tmpstream);
	if (jpc_enc_encpkts(enc, enc->tmpstream)) {
		return -1;
	}
	return 0;
}
