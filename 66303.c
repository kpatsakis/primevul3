GF_Err dimC_dump(GF_Box *a, FILE * trace)
{
	GF_DIMSSceneConfigBox *p = (GF_DIMSSceneConfigBox *)a;
	gf_isom_box_dump_start(a, "DIMSSceneConfigBox", trace);
	fprintf(trace, "profile=\"%d\" level=\"%d\" pathComponents=\"%d\" useFullRequestHosts=\"%d\" streamType=\"%d\" containsRedundant=\"%d\" textEncoding=\"%s\" contentEncoding=\"%s\" >\n",
	        p->profile, p->level, p->pathComponents, p->fullRequestHost, p->streamType, p->containsRedundant, p->textEncoding, p->contentEncoding);
	gf_isom_box_dump_done("DIMSSceneConfigBox", a, trace);
	return GF_OK;
}
