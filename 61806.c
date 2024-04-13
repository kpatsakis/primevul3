static void jas_icclut16_dump(jas_iccattrval_t *attrval, FILE *out)
{
	jas_icclut16_t *lut16 = &attrval->data.lut16;
	int i;
	int j;
	fprintf(out, "numinchans=%d, numoutchans=%d, clutlen=%d\n",
	  lut16->numinchans, lut16->numoutchans, lut16->clutlen);
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 3; ++j) {
			fprintf(out, "e[%d][%d]=%f ", i, j, lut16->e[i][j] / 65536.0);
		}
		fprintf(out, "\n");
	}
	fprintf(out, "numintabents=%"PRIuFAST16", numouttabents=%"PRIuFAST16"\n",
	  lut16->numintabents, lut16->numouttabents);
}
