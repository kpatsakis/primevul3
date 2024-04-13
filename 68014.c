u32 gf_isom_get_avc_svc_type(GF_ISOFile *the_file, u32 trackNumber, u32 DescriptionIndex)
{
	u32 type;
	GF_TrackBox *trak;
	GF_MPEGVisualSampleEntryBox *entry;
	trak = gf_isom_get_track_from_file(the_file, trackNumber);
	if (!trak || !trak->Media || !DescriptionIndex) return GF_ISOM_AVCTYPE_NONE;
	if (trak->Media->handler->handlerType != GF_ISOM_MEDIA_VISUAL) return GF_ISOM_AVCTYPE_NONE;
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
	case GF_ISOM_BOX_TYPE_AVC1:
	case GF_ISOM_BOX_TYPE_AVC2:
	case GF_ISOM_BOX_TYPE_AVC3:
	case GF_ISOM_BOX_TYPE_AVC4:
	case GF_ISOM_BOX_TYPE_SVC1:
	case GF_ISOM_BOX_TYPE_MVC1:
		break;
	default:
		return GF_ISOM_AVCTYPE_NONE;
	}
	if (entry->avc_config && !entry->svc_config && !entry->mvc_config) return GF_ISOM_AVCTYPE_AVC_ONLY;
	if (entry->avc_config && entry->svc_config) return GF_ISOM_AVCTYPE_AVC_SVC;
	if (entry->avc_config && entry->mvc_config) return GF_ISOM_AVCTYPE_AVC_MVC;
	if (!entry->avc_config && entry->svc_config) return GF_ISOM_AVCTYPE_SVC_ONLY;
	if (!entry->avc_config && entry->mvc_config) return GF_ISOM_AVCTYPE_MVC_ONLY;
	return GF_ISOM_AVCTYPE_NONE;
}
