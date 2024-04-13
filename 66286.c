GF_Err auxc_dump(GF_Box *a, FILE * trace)
{
	GF_AuxiliaryTypePropertyBox *ptr = (GF_AuxiliaryTypePropertyBox *)a;

	gf_isom_box_dump_start(a, "AuxiliaryTypePropertyBox", trace);
	fprintf(trace, "aux_type=\"%s\" ", ptr->aux_urn);
	dump_data_attribute(trace, "aux_subtype", ptr->data, ptr->data_size);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("AuxiliaryTypePropertyBox", a, trace);
	return GF_OK;
}
