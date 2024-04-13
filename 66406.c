GF_Err piff_pssh_dump(GF_Box *a, FILE * trace)
{
	GF_PIFFProtectionSystemHeaderBox *ptr = (GF_PIFFProtectionSystemHeaderBox*) a;
	if (!a) return GF_BAD_PARAM;

	gf_isom_box_dump_start(a, "PIFFProtectionSystemHeaderBox", trace);

	fprintf(trace, "SystemID=\"");
	dump_data_hex(trace, (char *) ptr->SystemID, 16);
	fprintf(trace, "\" PrivateData=\"");
	dump_data_hex(trace, (char *) ptr->private_data, ptr->private_data_size);
	fprintf(trace, "\">\n");
	gf_isom_box_dump_done("PIFFProtectionSystemHeaderBox", a, trace);
	return GF_OK;
}
