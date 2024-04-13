GF_Err sinf_dump(GF_Box *a, FILE * trace)
{
	GF_ProtectionSchemeInfoBox *p;
	p = (GF_ProtectionSchemeInfoBox *)a;
	gf_isom_box_dump_start(a, "ProtectionSchemeInfoBox", trace);
	fprintf(trace, ">\n");
	if (p->size)
		gf_isom_box_dump_ex(p->original_format, trace, GF_ISOM_BOX_TYPE_FRMA);
	if (p->size)
		gf_isom_box_dump_ex(p->scheme_type, trace, GF_ISOM_BOX_TYPE_SCHM);
	if (p->size)
		gf_isom_box_dump_ex(p->info, trace, GF_ISOM_BOX_TYPE_SCHI);
	gf_isom_box_dump_done("ProtectionSchemeInfoBox", a, trace);
	return GF_OK;
}
