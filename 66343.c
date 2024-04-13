GF_Err gnrm_dump(GF_Box *a, FILE * trace)
{
	GF_GenericSampleEntryBox *p = (GF_GenericSampleEntryBox *)a;
	if (p->EntryType)
		a->type = p->EntryType;

	gf_isom_box_dump_start(a, "SampleDescriptionBox", trace);
	fprintf(trace, "DataReferenceIndex=\"%d\" ExtensionDataSize=\"%d\">\n", p->dataReferenceIndex, p->data_size);
	a->type = GF_ISOM_BOX_TYPE_GNRM;
	gf_isom_box_dump_done("SampleDescriptionBox", a, trace);
	return GF_OK;
}
