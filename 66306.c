GF_Err dinf_dump(GF_Box *a, FILE * trace)
{
	GF_DataInformationBox *p;
	p = (GF_DataInformationBox *)a;
	gf_isom_box_dump_start(a, "DataInformationBox", trace);
	fprintf(trace, ">\n");
	if (p->size)
		gf_isom_box_dump_ex(p->dref, trace, GF_ISOM_BOX_TYPE_DREF);

	gf_isom_box_dump_done("DataInformationBox", a, trace);
	return GF_OK;
}
