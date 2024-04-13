void jpc_enc_dump(jpc_enc_t *enc)
{
	jpc_enc_tile_t *tile;
	jpc_enc_tcmpt_t *tcmpt;
	jpc_enc_rlvl_t *rlvl;
	jpc_enc_band_t *band;
	jpc_enc_prc_t *prc;
	jpc_enc_cblk_t *cblk;
	uint_fast16_t cmptno;
	uint_fast16_t rlvlno;
	uint_fast16_t bandno;
	uint_fast32_t prcno;
	uint_fast32_t cblkno;

	tile = enc->curtile;

	for (cmptno = 0, tcmpt = tile->tcmpts; cmptno < tile->numtcmpts; ++cmptno,
	  ++tcmpt) {
		jas_eprintf("  tcmpt %5d %5d %5d %5d\n", jas_seq2d_xstart(tcmpt->data), jas_seq2d_ystart(tcmpt->data), jas_seq2d_xend(tcmpt->data), jas_seq2d_yend(tcmpt->data));
		for (rlvlno = 0, rlvl = tcmpt->rlvls; rlvlno < tcmpt->numrlvls;
		  ++rlvlno, ++rlvl) {
			jas_eprintf("    rlvl %5d %5d %5d %5d\n", rlvl->tlx, rlvl->tly, rlvl->brx, rlvl->bry);
			for (bandno = 0, band = rlvl->bands; bandno < rlvl->numbands;
			  ++bandno, ++band) {
				if (!band->data) {
					continue;
				}
				jas_eprintf("      band %5d %5d %5d %5d\n", jas_seq2d_xstart(band->data), jas_seq2d_ystart(band->data), jas_seq2d_xend(band->data), jas_seq2d_yend(band->data));
				for (prcno = 0, prc = band->prcs; prcno < rlvl->numprcs;
				  ++prcno, ++prc) {
					jas_eprintf("        prc %5d %5d %5d %5d (%5d %5d)\n", prc->tlx, prc->tly, prc->brx, prc->bry, prc->brx - prc->tlx, prc->bry - prc->tly);
					if (!prc->cblks) {
						continue;
					}
					for (cblkno = 0, cblk = prc->cblks; cblkno < prc->numcblks;
					  ++cblkno, ++cblk) {
						jas_eprintf("         cblk %5d %5d %5d %5d\n", jas_seq2d_xstart(cblk->data), jas_seq2d_ystart(cblk->data), jas_seq2d_xend(cblk->data), jas_seq2d_yend(cblk->data));
					}
				}
			}
		}
	}
}
