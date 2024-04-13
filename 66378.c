GF_Err mdhd_dump(GF_Box *a, FILE * trace)
{
	GF_MediaHeaderBox *p;

	p = (GF_MediaHeaderBox *)a;
	gf_isom_box_dump_start(a, "MediaHeaderBox", trace);
	fprintf(trace, "CreationTime=\""LLD"\" ", LLD_CAST p->creationTime);
	fprintf(trace, "ModificationTime=\""LLD"\" ", LLD_CAST p->modificationTime);
	fprintf(trace, "TimeScale=\"%d\" ", p->timeScale);
	fprintf(trace, "Duration=\""LLD"\" ", LLD_CAST p->duration);
	fprintf(trace, "LanguageCode=\"%c%c%c\">\n", p->packedLanguage[0], p->packedLanguage[1], p->packedLanguage[2]);
	gf_isom_box_dump_done("MediaHeaderBox", a, trace);
	return GF_OK;
}
