GF_Err ainf_dump(GF_Box *a, FILE * trace)
{
	GF_AssetInformationBox *p = (GF_AssetInformationBox *) a;

	gf_isom_box_dump_start(a, "AssetInformationBox", trace);

	fprintf(trace, "profile_version=\"%d\" APID=\"%s\">\n", p->profile_version, p->APID);
	gf_isom_box_dump_done("AssetInformationBox", a, trace);
	return GF_OK;
}
