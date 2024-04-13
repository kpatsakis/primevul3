static void jas_icccurv_dump(jas_iccattrval_t *attrval, FILE *out)
{
	int i;
	jas_icccurv_t *curv = &attrval->data.curv;
	fprintf(out, "number of entries = %"PRIuFAST32"\n", curv->numents);
	if (curv->numents == 1) {
		fprintf(out, "gamma = %f\n", curv->ents[0] / 256.0);
	} else {
		for (i = 0; i < JAS_CAST(int, curv->numents); ++i) {
			if (i < 3 || i >= JAS_CAST(int, curv->numents) - 3) {
				fprintf(out, "entry[%d] = %f\n", i, curv->ents[i] / 65535.0);
			}
		}
	}
}
