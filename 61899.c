static jp2_dec_t *jp2_dec_create(void)
{
	jp2_dec_t *dec;

	if (!(dec = jas_malloc(sizeof(jp2_dec_t)))) {
		return 0;
	}
	dec->ihdr = 0;
	dec->bpcc = 0;
	dec->cdef = 0;
	dec->pclr = 0;
	dec->image = 0;
	dec->chantocmptlut = 0;
	dec->cmap = 0;
	dec->colr = 0;
	return dec;
}
