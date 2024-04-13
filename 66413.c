GF_Err reftype_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_TrackReferenceTypeBox *p = (GF_TrackReferenceTypeBox *)a;
	if (!p->reference_type) return GF_OK;
	p->type = p->reference_type;

	gf_isom_box_dump_start(a, "TrackReferenceTypeBox", trace);
	fprintf(trace, ">\n");
	for (i=0; i<p->trackIDCount; i++) {
		fprintf(trace, "<TrackReferenceEntry TrackID=\"%d\"/>\n", p->trackIDs[i]);
	}
	if (!p->size)
		fprintf(trace, "<TrackReferenceEntry TrackID=\"\"/>\n");

	gf_isom_box_dump_done("TrackReferenceTypeBox", a, trace);
	p->type = GF_ISOM_BOX_TYPE_REFT;
	return GF_OK;
}
