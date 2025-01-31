static int jpc_dec_dump(jpc_dec_t *dec, FILE *out)
{
	jpc_dec_tile_t *tile;
	int tileno;
	jpc_dec_tcomp_t *tcomp;
	int compno;
	jpc_dec_rlvl_t *rlvl;
	int rlvlno;
	jpc_dec_band_t *band;
	int bandno;
	jpc_dec_prc_t *prc;
	int prcno;
	jpc_dec_cblk_t *cblk;
	int cblkno;

	assert(!dec->numtiles || dec->tiles);
	for (tileno = 0, tile = dec->tiles; tileno < dec->numtiles;
	  ++tileno, ++tile) {
		assert(!dec->numcomps || tile->tcomps);
		for (compno = 0, tcomp = tile->tcomps; compno < dec->numcomps;
		  ++compno, ++tcomp) {
			for (rlvlno = 0, rlvl = tcomp->rlvls; rlvlno <
			  tcomp->numrlvls; ++rlvlno, ++rlvl) {
				fprintf(out, "RESOLUTION LEVEL %d\n", rlvlno);
				fprintf(out, "xs = %"PRIuFAST32", ys = %"PRIuFAST32", xe = %"PRIuFAST32", ye = %"PRIuFAST32", w = %"PRIuFAST32", h = %"PRIuFAST32"\n",
				  rlvl->xstart, rlvl->ystart, rlvl->xend, rlvl->yend,
				  rlvl->xend - rlvl->xstart, rlvl->yend - rlvl->ystart);
				assert(!rlvl->numbands || rlvl->bands);
				for (bandno = 0, band = rlvl->bands;
				  bandno < rlvl->numbands; ++bandno, ++band) {
					fprintf(out, "BAND %d\n", bandno);
					if (!band->data) {
						fprintf(out, "band has no data (null pointer)\n");
						assert(!band->prcs);
						continue;
					}
					fprintf(out, "xs = %"PRIiFAST32", ys = %"PRIiFAST32", xe = %"PRIiFAST32", ye = %"PRIiFAST32", w = %"PRIiFAST32", h = %"PRIiFAST32"\n",
					  jas_seq2d_xstart(band->data),
					  jas_seq2d_ystart(band->data),
					  jas_seq2d_xend(band->data),
					  jas_seq2d_yend(band->data),
					  jas_seq2d_xend(band->data) -
					  jas_seq2d_xstart(band->data),
					  jas_seq2d_yend(band->data) -
					  jas_seq2d_ystart(band->data));
					assert(!rlvl->numprcs || band->prcs);
					for (prcno = 0, prc = band->prcs;
					  prcno < rlvl->numprcs; ++prcno,
					  ++prc) {
						fprintf(out, "CODE BLOCK GROUP %d\n", prcno);
						fprintf(out, "xs = %"PRIuFAST32", ys = %"PRIuFAST32", xe = %"PRIuFAST32", ye = %"PRIuFAST32", w = %"PRIuFAST32", h = %"PRIuFAST32"\n",
						  prc->xstart, prc->ystart, prc->xend, prc->yend,
						  prc->xend - prc->xstart, prc->yend - prc->ystart);
						assert(!prc->numcblks || prc->cblks);
						for (cblkno = 0, cblk =
						  prc->cblks; cblkno <
						  prc->numcblks; ++cblkno,
						  ++cblk) {
							fprintf(out, "CODE BLOCK %d\n", cblkno);
							fprintf(out, "xs = %"PRIiFAST32", ys = %"PRIiFAST32", xe = %"PRIiFAST32", ye = %"PRIiFAST32", w = %"PRIiFAST32", h = %"PRIiFAST32"\n",
							  jas_seq2d_xstart(cblk->data),
							  jas_seq2d_ystart(cblk->data),
							  jas_seq2d_xend(cblk->data),
							  jas_seq2d_yend(cblk->data),
							  jas_seq2d_xend(cblk->data) -
							  jas_seq2d_xstart(cblk->data),
							  jas_seq2d_yend(cblk->data) -
							  jas_seq2d_ystart(cblk->data));
						}
					}
				}
			}
		}
	}

	return 0;
}
