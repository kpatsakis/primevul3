int jpc_tsfb_analyze2(jpc_tsfb_t *tsfb, int *a, int xstart, int ystart,
  int width, int height, int stride, int numlvls)
{
	if (width > 0 && height > 0) {
		if ((*tsfb->qmfb->analyze)(a, xstart, ystart, width, height, stride))
			return -1;
		if (numlvls > 0) {
			if (jpc_tsfb_analyze2(tsfb, a, JPC_CEILDIVPOW2(xstart,
			  1), JPC_CEILDIVPOW2(ystart, 1), JPC_CEILDIVPOW2(
			  xstart + width, 1) - JPC_CEILDIVPOW2(xstart, 1),
			  JPC_CEILDIVPOW2(ystart + height, 1) -
			  JPC_CEILDIVPOW2(ystart, 1), stride, numlvls - 1)) {
				return -1;
			}
		}
	}
	return 0;
}
