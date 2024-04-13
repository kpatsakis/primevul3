GF_Err minf_dump(GF_Box *a, FILE * trace)
{
	GF_MediaInformationBox *p;

	p = (GF_MediaInformationBox *)a;
	gf_isom_box_dump_start(a, "MediaInformationBox", trace);
	fprintf(trace, ">\n");

	if (p->size)
		gf_isom_box_dump_ex(p->InfoHeader, trace, GF_ISOM_BOX_TYPE_NMHD);
	if (p->size)
		gf_isom_box_dump_ex(p->dataInformation, trace, GF_ISOM_BOX_TYPE_DINF);
	if (p->size)
		gf_isom_box_dump_ex(p->sampleTable, trace, GF_ISOM_BOX_TYPE_STBL);

	gf_isom_box_dump_done("MediaInformationBox", a, trace);
	return GF_OK;
}
