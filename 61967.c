int dump_passes(jpc_enc_pass_t *passes, int numpasses, jpc_enc_cblk_t *cblk)
{
	jpc_enc_pass_t *pass;
	int i;
	jas_stream_memobj_t *smo;

	smo = cblk->stream->obj_;

	pass = passes;
	for (i = 0; i < numpasses; ++i) {
		jas_eprintf("start=%d end=%d type=%d term=%d lyrno=%d firstchar=%02x size=%ld pos=%ld\n",
		  (int)pass->start, (int)pass->end, (int)pass->type, (int)pass->term, (int)pass->lyrno,
		  smo->buf_[pass->start], (long)smo->len_, (long)smo->pos_);
#if 0
		jas_memdump(stderr, &smo->buf_[pass->start], pass->end - pass->start);
#endif
		++pass;
	}
	return 0;
}
