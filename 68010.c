GF_Err gf_isom_avc_config_new(GF_ISOFile *the_file, u32 trackNumber, GF_AVCConfig *cfg, char *URLname, char *URNname, u32 *outDescriptionIndex)
{
	GF_TrackBox *trak;
	GF_Err e;
	u32 dataRefIndex;
	GF_MPEGVisualSampleEntryBox *entry;

	e = CanAccessMovie(the_file, GF_ISOM_OPEN_WRITE);
	if (e) return e;

	trak = gf_isom_get_track_from_file(the_file, trackNumber);
	if (!trak || !trak->Media || !cfg) return GF_BAD_PARAM;

	e = Media_FindDataRef(trak->Media->information->dataInformation->dref, URLname, URNname, &dataRefIndex);
	if (e) return e;
	if (!dataRefIndex) {
		e = Media_CreateDataRef(trak->Media->information->dataInformation->dref, URLname, URNname, &dataRefIndex);
		if (e) return e;
	}
	if (!the_file->keep_utc)
		trak->Media->mediaHeader->modificationTime = gf_isom_get_mp4time();

	entry = (GF_MPEGVisualSampleEntryBox *) gf_isom_box_new(GF_ISOM_BOX_TYPE_AVC1);
	if (!entry) return GF_OUT_OF_MEM;
	entry->avc_config = (GF_AVCConfigurationBox*)gf_isom_box_new(GF_ISOM_BOX_TYPE_AVCC);
	entry->avc_config->config = AVC_DuplicateConfig(cfg);
	entry->dataReferenceIndex = dataRefIndex;
	e = gf_list_add(trak->Media->information->sampleTable->SampleDescription->other_boxes, entry);
	*outDescriptionIndex = gf_list_count(trak->Media->information->sampleTable->SampleDescription->other_boxes);
	AVC_RewriteESDescriptor(entry);
	return e;
}
