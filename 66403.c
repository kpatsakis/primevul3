GF_Err pcrb_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_PcrInfoBox *p = (GF_PcrInfoBox *)a;
	gf_isom_box_dump_start(a, "MPEG2TSPCRInfoBox", trace);
	fprintf(trace, "subsegment_count=\"%d\">\n", p->subsegment_count);

	for (i=0; i<p->subsegment_count; i++) {
		fprintf(trace, "<PCRInfo PCR=\""LLU"\" />\n", p->pcr_values[i]);
	}
	if (!p->size) {
		fprintf(trace, "<PCRInfo PCR=\"\" />\n");
	}
	gf_isom_box_dump_done("MPEG2TSPCRInfoBox", a, trace);
	return GF_OK;
}
