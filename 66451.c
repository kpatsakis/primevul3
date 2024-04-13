GF_Err tkhd_dump(GF_Box *a, FILE * trace)
{
	GF_TrackHeaderBox *p;
	p = (GF_TrackHeaderBox *)a;
	gf_isom_box_dump_start(a, "TrackHeaderBox", trace);

	fprintf(trace, "CreationTime=\""LLD"\" ModificationTime=\""LLD"\" TrackID=\"%u\" Duration=\""LLD"\"",
	        LLD_CAST p->creationTime, LLD_CAST p->modificationTime, p->trackID, LLD_CAST p->duration);

	if (p->alternate_group) fprintf(trace, " AlternateGroupID=\"%d\"", p->alternate_group);
	if (p->volume) {
		fprintf(trace, " Volume=\"%.2f\"", (Float)p->volume / 256);
	} else if (p->width || p->height) {
		fprintf(trace, " Width=\"%.2f\" Height=\"%.2f\"", (Float)p->width / 65536, (Float)p->height / 65536);
		if (p->layer) fprintf(trace, " Layer=\"%d\"", p->layer);
	}
	fprintf(trace, ">\n");
	if (p->width || p->height) {
		fprintf(trace, "<Matrix m11=\"0x%.8x\" m12=\"0x%.8x\" m13=\"0x%.8x\" ", p->matrix[0], p->matrix[1], p->matrix[2]);
		fprintf(trace, "m21=\"0x%.8x\" m22=\"0x%.8x\" m23=\"0x%.8x\" ", p->matrix[3], p->matrix[4], p->matrix[5]);
		fprintf(trace, "m31=\"0x%.8x\" m32=\"0x%.8x\" m33=\"0x%.8x\"/>\n", p->matrix[6], p->matrix[7], p->matrix[8]);
	}

	gf_isom_box_dump_done("TrackHeaderBox", a, trace);
	return GF_OK;
}
