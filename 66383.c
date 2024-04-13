GF_Err metx_dump(GF_Box *a, FILE * trace)
{
	GF_MetaDataSampleEntryBox *ptr = (GF_MetaDataSampleEntryBox*)a;
	const char *name;
	switch (ptr->type) {
	case GF_ISOM_BOX_TYPE_METX:
		name = "XMLMetaDataSampleEntryBox";
		break;
	case GF_ISOM_BOX_TYPE_METT:
		name = "TextMetaDataSampleEntryBox";
		break;
	case GF_ISOM_BOX_TYPE_SBTT:
		name = "SubtitleSampleEntryBox";
		break;
	case GF_ISOM_BOX_TYPE_STXT:
		name = "SimpleTextSampleEntryBox";
		break;
	case GF_ISOM_BOX_TYPE_STPP:
		name = "XMLSubtitleSampleEntryBox";
		break;
	default:
		name = "UnknownTextSampleEntryBox";
		break;
	}
	gf_isom_box_dump_start(a, name, trace);

	if (ptr->type==GF_ISOM_BOX_TYPE_METX) {
		fprintf(trace, "namespace=\"%s\" ", ptr->xml_namespace);
		if (ptr->xml_schema_loc) fprintf(trace, "schema_location=\"%s\" ", ptr->xml_schema_loc);
		if (ptr->content_encoding) fprintf(trace, "content_encoding=\"%s\" ", ptr->content_encoding);

	} else if (ptr->type==GF_ISOM_BOX_TYPE_STPP) {
		fprintf(trace, "namespace=\"%s\" ", ptr->xml_namespace);
		if (ptr->xml_schema_loc) fprintf(trace, "schema_location=\"%s\" ", ptr->xml_schema_loc);
		if (ptr->mime_type) fprintf(trace, "auxiliary_mime_types=\"%s\" ", ptr->mime_type);
	}
	else {
		fprintf(trace, "mime_type=\"%s\" ", ptr->mime_type);
		if (ptr->content_encoding) fprintf(trace, "content_encoding=\"%s\" ", ptr->content_encoding);
	}
	fprintf(trace, ">\n");

	if ((ptr->type!=GF_ISOM_BOX_TYPE_METX) && (ptr->type!=GF_ISOM_BOX_TYPE_STPP) ) {
		if (ptr->config) gf_isom_box_dump(ptr->config, trace);
	}
	gf_isom_box_array_dump(ptr->protections, trace);

	gf_isom_box_dump_done(name, a, trace);
	return GF_OK;
}
