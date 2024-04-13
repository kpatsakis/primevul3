void dump_layeringinfo(jpc_enc_t *enc)
{

	jpc_enc_tcmpt_t *tcmpt;
	int tcmptno;
	jpc_enc_rlvl_t *rlvl;
	int rlvlno;
	jpc_enc_band_t *band;
	int bandno;
	jpc_enc_prc_t *prc;
	int prcno;
	jpc_enc_cblk_t *cblk;
	int cblkno;
	jpc_enc_pass_t *pass;
	int passno;
	int lyrno;
	jpc_enc_tile_t *tile;

	tile = enc->curtile;

	for (lyrno = 0; lyrno < tile->numlyrs; ++lyrno) {
		jas_eprintf("lyrno = %02d\n", lyrno);
		for (tcmptno = 0, tcmpt = tile->tcmpts; tcmptno < tile->numtcmpts;
		  ++tcmptno, ++tcmpt) {
			for (rlvlno = 0, rlvl = tcmpt->rlvls; rlvlno < tcmpt->numrlvls;
			  ++rlvlno, ++rlvl) {
				if (!rlvl->bands) {
					continue;
				}
				for (bandno = 0, band = rlvl->bands; bandno < rlvl->numbands;
				  ++bandno, ++band) {
					if (!band->data) {
						continue;
					}
					for (prcno = 0, prc = band->prcs; prcno < rlvl->numprcs;
					  ++prcno, ++prc) {
						if (!prc->cblks) {
							continue;
						}
						for (cblkno = 0, cblk = prc->cblks; cblkno <
						  prc->numcblks; ++cblkno, ++cblk) {
							for (passno = 0, pass = cblk->passes; passno <
							  cblk->numpasses && pass->lyrno == lyrno;
							  ++passno, ++pass) {
								jas_eprintf("lyrno=%02d cmptno=%02d rlvlno=%02d bandno=%02d prcno=%02d cblkno=%03d passno=%03d\n", lyrno, tcmptno, rlvlno, bandno, prcno, cblkno, passno);
							}
						}
					}
				}
			}
		}
	}
}
