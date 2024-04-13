GF_Err ftyp_dump(GF_Box *a, FILE * trace)
{
	GF_FileTypeBox *p;
	u32 i;

	p = (GF_FileTypeBox *)a;
	gf_isom_box_dump_start(a, (a->type == GF_ISOM_BOX_TYPE_FTYP ? "FileTypeBox" : "SegmentTypeBox"), trace);
	fprintf(trace, "MajorBrand=\"%s\" MinorVersion=\"%d\">\n", gf_4cc_to_str(p->majorBrand), p->minorVersion);

	for (i=0; i<p->altCount; i++) {
		fprintf(trace, "<BrandEntry AlternateBrand=\"%s\"/>\n", gf_4cc_to_str(p->altBrand[i]));
	}
	if (!p->type) {
		fprintf(trace, "<BrandEntry AlternateBrand=\"4CC\"/>\n");
	}
	gf_isom_box_dump_done((a->type == GF_ISOM_BOX_TYPE_FTYP ? "FileTypeBox" : "SegmentTypeBox"), a, trace);
	return GF_OK;
}
