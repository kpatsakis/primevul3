static void jas_icclut8_dump(jas_iccattrval_t *attrval, FILE *out)
{
	jas_icclut8_t *lut8 = &attrval->data.lut8;
	int i;
	int j;
	fprintf(out, "numinchans=%d, numoutchans=%d, clutlen=%d\n",
	  lut8->numinchans, lut8->numoutchans, lut8->clutlen);
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 3; ++j) {
			fprintf(out, "e[%d][%d]=%f ", i, j, lut8->e[i][j] / 65536.0);
		}
		fprintf(out, "\n");
	}
	fprintf(out, "numintabents=%"PRIuFAST16", numouttabents=%"PRIuFAST16"\n",
	  lut8->numintabents, lut8->numouttabents);
}
