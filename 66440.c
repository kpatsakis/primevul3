GF_Err stsz_dump(GF_Box *a, FILE * trace)
{
	GF_SampleSizeBox *p;
	u32 i;
	p = (GF_SampleSizeBox *)a;

	if (a->type == GF_ISOM_BOX_TYPE_STSZ) {
		gf_isom_box_dump_start(a, "SampleSizeBox", trace);
	}
	else {
		gf_isom_box_dump_start(a, "CompactSampleSizeBox", trace);
	}

	fprintf(trace, "SampleCount=\"%d\"",  p->sampleCount);
	if (a->type == GF_ISOM_BOX_TYPE_STSZ) {
		if (p->sampleSize) {
			fprintf(trace, " ConstantSampleSize=\"%d\"", p->sampleSize);
		}
	} else {
		fprintf(trace, " SampleSizeBits=\"%d\"", p->sampleSize);
	}
	fprintf(trace, ">\n");

	if ((a->type != GF_ISOM_BOX_TYPE_STSZ) || !p->sampleSize) {
		if (!p->sizes && p->size) {
			fprintf(trace, "<!--WARNING: No Sample Size indications-->\n");
		} else {
			for (i=0; i<p->sampleCount; i++) {
				fprintf(trace, "<SampleSizeEntry Size=\"%d\"/>\n", p->sizes[i]);
			}
		}
	}
	if (!p->size) {
		fprintf(trace, "<SampleSizeEntry Size=\"\"/>\n");
	}
	gf_isom_box_dump_done((a->type == GF_ISOM_BOX_TYPE_STSZ) ? "SampleSizeBox" : "CompactSampleSizeBox", a, trace);
	return GF_OK;
}
