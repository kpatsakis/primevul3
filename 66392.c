GF_Err mvhd_dump(GF_Box *a, FILE * trace)
{
	GF_MovieHeaderBox *p;

	p = (GF_MovieHeaderBox *) a;

	gf_isom_box_dump_start(a, "MovieHeaderBox", trace);
	fprintf(trace, "CreationTime=\""LLD"\" ", LLD_CAST p->creationTime);
	fprintf(trace, "ModificationTime=\""LLD"\" ", LLD_CAST p->modificationTime);
	fprintf(trace, "TimeScale=\"%d\" ", p->timeScale);
	fprintf(trace, "Duration=\""LLD"\" ", LLD_CAST p->duration);
	fprintf(trace, "NextTrackID=\"%d\">\n", p->nextTrackID);

	gf_isom_box_dump_done("MovieHeaderBox", a, trace);
	return GF_OK;
}
