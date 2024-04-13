int jpc_tsfb_getbands(jpc_tsfb_t *tsfb, uint_fast32_t xstart,
  uint_fast32_t ystart, uint_fast32_t xend, uint_fast32_t yend,
  jpc_tsfb_band_t *bands)
{
	jpc_tsfb_band_t *band;

	band = bands;
	if (tsfb->numlvls > 0) {
		jpc_tsfb_getbands2(tsfb, xstart, ystart, xstart, ystart, xend, yend,
		  &band, tsfb->numlvls);
	} else {

		band->xstart = xstart;
		band->ystart = ystart;
		band->xend = xend;
		band->yend = yend;
		band->locxstart = xstart;
		band->locystart = ystart;
		band->locxend = band->locxstart + band->xend - band->xstart;
		band->locyend = band->locystart + band->yend - band->ystart;
		band->orient = JPC_TSFB_LL;
		band->synenergywt = JPC_FIX_ONE;
		++band;
	}
	return band - bands;
}
