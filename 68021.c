Bool gf_isom_is_nalu_based_entry(GF_MediaBox *mdia, GF_SampleEntryBox *_entry)
{
	GF_MPEGVisualSampleEntryBox *entry;
	if (mdia->handler->handlerType != GF_ISOM_MEDIA_VISUAL) return GF_FALSE;
	switch (_entry->type) {
	case GF_ISOM_BOX_TYPE_AVC1:
	case GF_ISOM_BOX_TYPE_AVC2:
	case GF_ISOM_BOX_TYPE_AVC3:
	case GF_ISOM_BOX_TYPE_AVC4:
	case GF_ISOM_BOX_TYPE_SVC1:
	case GF_ISOM_BOX_TYPE_SVC2:
	case GF_ISOM_BOX_TYPE_MVC1:
	case GF_ISOM_BOX_TYPE_MVC2:
	case GF_ISOM_BOX_TYPE_HVC1:
	case GF_ISOM_BOX_TYPE_HEV1:
	case GF_ISOM_BOX_TYPE_HVC2:
	case GF_ISOM_BOX_TYPE_HEV2:
	case GF_ISOM_BOX_TYPE_LHV1:
	case GF_ISOM_BOX_TYPE_LHE1:
	case GF_ISOM_BOX_TYPE_MHV1:
	case GF_ISOM_BOX_TYPE_MHC1:
	case GF_ISOM_BOX_TYPE_HVT1:
	case GF_ISOM_BOX_TYPE_LHT1:
		return GF_TRUE;
	case GF_ISOM_BOX_TYPE_GNRV:
	case GF_ISOM_BOX_TYPE_GNRA:
	case GF_ISOM_BOX_TYPE_GNRM:
		return GF_FALSE;
	default:
		break;
	}
	entry = (GF_MPEGVisualSampleEntryBox*)_entry;
	if (!entry) return GF_FALSE;
	if (entry->avc_config || entry->svc_config || entry->mvc_config || entry->hevc_config || entry->lhvc_config) return GF_TRUE;
	return GF_FALSE;
}
