GF_Err cslg_dump(GF_Box *a, FILE * trace)
{
	GF_CompositionToDecodeBox *p;

	p = (GF_CompositionToDecodeBox *)a;
	gf_isom_box_dump_start(a, "CompositionToDecodeBox", trace);
	fprintf(trace, "compositionToDTSShift=\"%d\" leastDecodeToDisplayDelta=\"%d\" compositionStartTime=\"%d\" compositionEndTime=\"%d\">\n", p->leastDecodeToDisplayDelta, p->greatestDecodeToDisplayDelta, p->compositionStartTime, p->compositionEndTime);
	gf_isom_box_dump_done("CompositionToDecodeBox", a, trace);
	return GF_OK;
}
