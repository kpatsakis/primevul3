static jas_seqent_t pgx_wordtoint(uint_fast32_t v, int prec, bool sgnd)
{
	jas_seqent_t ret;
	v &= (1 << prec) - 1;
	ret = (sgnd && (v & (1 << (prec - 1)))) ? (v - (1 << prec)) : v;
	return ret;
}
