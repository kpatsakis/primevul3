GF_Err sidx_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_SegmentIndexBox *p = (GF_SegmentIndexBox *)a;
	gf_isom_box_dump_start(a, "SegmentIndexBox", trace);
	fprintf(trace, "reference_ID=\"%d\" timescale=\"%d\" earliest_presentation_time=\""LLD"\" first_offset=\""LLD"\" ", p->reference_ID, p->timescale, p->earliest_presentation_time, p->first_offset);

	fprintf(trace, ">\n");
	for (i=0; i<p->nb_refs; i++) {
		fprintf(trace, "<Reference type=\"%d\" size=\"%d\" duration=\"%d\" startsWithSAP=\"%d\" SAP_type=\"%d\" SAPDeltaTime=\"%d\"/>\n", p->refs[i].reference_type, p->refs[i].reference_size, p->refs[i].subsegment_duration, p->refs[i].starts_with_SAP, p->refs[i].SAP_type, p->refs[i].SAP_delta_time);
	}
	if (!p->size) {
		fprintf(trace, "<Reference type=\"\" size=\"\" duration=\"\" startsWithSAP=\"\" SAP_type=\"\" SAPDeltaTime=\"\"/>\n");
	}
	gf_isom_box_dump_done("SegmentIndexBox", a, trace);
	return GF_OK;
}
