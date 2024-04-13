GF_Err gnra_dump(GF_Box *a, FILE * trace)
{
	GF_GenericAudioSampleEntryBox *p = (GF_GenericAudioSampleEntryBox *)a;
	if (p->EntryType)
		a->type = p->EntryType;

	gf_isom_box_dump_start(a, "AudioSampleDescriptionBox", trace);
	fprintf(trace, "DataReferenceIndex=\"%d\" Version=\"%d\" Revision=\"%d\" Vendor=\"%d\" ChannelCount=\"%d\" BitsPerSample=\"%d\" Samplerate=\"%d\">\n",
	        p->dataReferenceIndex, p->version, p->revision, p->vendor, p->channel_count, p->bitspersample, p->samplerate_hi);
	a->type = GF_ISOM_BOX_TYPE_GNRA;
	gf_isom_box_dump_done("AudioSampleDescriptionBox", a, trace);
	return GF_OK;
}
