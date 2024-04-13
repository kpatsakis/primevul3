GF_Err gf_isom_dump(GF_ISOFile *mov, FILE * trace)
{
	u32 i;
	GF_Box *box;
	if (!mov || !trace) return GF_BAD_PARAM;

	use_dump_mode = mov->dump_mode_alloc;
	fprintf(trace, "<!--MP4Box dump trace-->\n");

	fprintf(trace, "<IsoMediaFile xmlns=\"urn:mpeg:isobmff:schema:file:2016\" Name=\"%s\">\n", mov->fileName);

	i=0;
	while ((box = (GF_Box *)gf_list_enum(mov->TopBoxes, &i))) {
		if (box->type==GF_ISOM_BOX_TYPE_UNKNOWN) {
			fprintf(trace, "<!--WARNING: Unknown Top-level Box Found -->\n");
		} else if (box->type==GF_ISOM_BOX_TYPE_UUID) {
		} else if (!gf_isom_box_is_file_level(box)) {
			fprintf(trace, "<!--ERROR: Invalid Top-level Box Found (\"%s\")-->\n", gf_4cc_to_str(box->type));
		}
		gf_isom_box_dump(box, trace);
	}
	fprintf(trace, "</IsoMediaFile>\n");
	return GF_OK;
}
