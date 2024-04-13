u32 gf_isom_get_hevc_lhvc_type(GF_ISOFile *the_file, u32 trackNumber, u32 DescriptionIndex)
{
	u32 type;
	GF_TrackBox *trak;
	GF_MPEGVisualSampleEntryBox *entry;
	trak = gf_isom_get_track_from_file(the_file, trackNumber);
	if (!trak || !trak->Media || !DescriptionIndex) return GF_ISOM_HEVCTYPE_NONE;
	if (trak->Media->handler->handlerType != GF_ISOM_MEDIA_VISUAL) return GF_ISOM_HEVCTYPE_NONE;
	entry = (GF_MPEGVisualSampleEntryBox*)gf_list_get(trak->Media->information->sampleTable->SampleDescription->other_boxes, DescriptionIndex-1);
	if (!entry) return GF_ISOM_AVCTYPE_NONE;
	type = entry->type;

	if (type == GF_ISOM_BOX_TYPE_ENCV) {
		GF_ProtectionSchemeInfoBox *sinf = (GF_ProtectionSchemeInfoBox *) gf_list_get(entry->protections, 0);
		if (sinf && sinf->original_format) type = sinf->original_format->data_format;
	}
	else if (type == GF_ISOM_BOX_TYPE_RESV) {
		if (entry->rinf && entry->rinf->original_format) type = entry->rinf->original_format->data_format;
	}

	switch (type) {
	case GF_ISOM_BOX_TYPE_HVC1:
	case GF_ISOM_BOX_TYPE_HEV1:
	case GF_ISOM_BOX_TYPE_HVC2:
	case GF_ISOM_BOX_TYPE_HEV2:
	case GF_ISOM_BOX_TYPE_LHV1:
	case GF_ISOM_BOX_TYPE_LHE1:
	case GF_ISOM_BOX_TYPE_HVT1:
		break;
	default:
		return GF_ISOM_HEVCTYPE_NONE;
	}
	if (entry->hevc_config && !entry->lhvc_config) return GF_ISOM_HEVCTYPE_HEVC_ONLY;
	if (entry->hevc_config && entry->lhvc_config) return GF_ISOM_HEVCTYPE_HEVC_LHVC;
	if (!entry->hevc_config && entry->lhvc_config) return GF_ISOM_HEVCTYPE_LHVC_ONLY;
	return GF_ISOM_HEVCTYPE_NONE;
}
