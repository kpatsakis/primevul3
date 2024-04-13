GF_Err pssh_dump(GF_Box *a, FILE * trace)
{
	GF_ProtectionSystemHeaderBox *ptr = (GF_ProtectionSystemHeaderBox*) a;
	if (!a) return GF_BAD_PARAM;

	gf_isom_box_dump_start(a, "ProtectionSystemHeaderBox", trace);

	fprintf(trace, "SystemID=\"");
	dump_data_hex(trace, (char *) ptr->SystemID, 16);
	fprintf(trace, "\">\n");

	if (ptr->KID_count) {
		u32 i;
		for (i=0; i<ptr->KID_count; i++) {
			fprintf(trace, " <PSSHKey KID=\"");
			dump_data_hex(trace, (char *) ptr->KIDs[i], 16);
			fprintf(trace, "\"/>\n");
		}
	}
	if (ptr->private_data_size) {
		fprintf(trace, " <PSSHData size=\"%d\" value=\"", ptr->private_data_size);
		dump_data_hex(trace, (char *) ptr->private_data, ptr->private_data_size);
		fprintf(trace, "\"/>\n");
	}
	if (!ptr->size) {
		fprintf(trace, " <PSSHKey KID=\"\"/>\n");
		fprintf(trace, " <PSSHData size=\"\" value=\"\"/>\n");
	}
	gf_isom_box_dump_done("ProtectionSystemHeaderBox", a, trace);
	return GF_OK;
}
