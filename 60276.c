int jpc_tsfb_analyze(jpc_tsfb_t *tsfb, jas_seq2d_t *a)
{
	return (tsfb->numlvls > 0) ? jpc_tsfb_analyze2(tsfb, jas_seq2d_getref(a,
	  jas_seq2d_xstart(a), jas_seq2d_ystart(a)), jas_seq2d_xstart(a),
	  jas_seq2d_ystart(a), jas_seq2d_width(a),
	  jas_seq2d_height(a), jas_seq2d_rowstep(a), tsfb->numlvls - 1) : 0;
}
