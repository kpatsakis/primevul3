GF_Err pdin_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_ProgressiveDownloadBox *p = (GF_ProgressiveDownloadBox *)a;
	gf_isom_box_dump_start(a, "ProgressiveDownloadBox", trace);
	fprintf(trace, ">\n");

	if (p->size) {
		for (i=0; i<p->count; i++) {
			fprintf(trace, "<DownloadInfo rate=\"%d\" estimatedTime=\"%d\" />\n", p->rates[i], p->times[i]);
		}
	} else {
		fprintf(trace, "<DownloadInfo rate=\"\" estimatedTime=\"\" />\n");
	}
	gf_isom_box_dump_done("ProgressiveDownloadBox", a, trace);
	return GF_OK;
}
