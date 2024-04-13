GF_Err stsd_AddBox(GF_Box *s, GF_Box *a)
{
	GF_UnknownBox *def;
	GF_SampleDescriptionBox *ptr = (GF_SampleDescriptionBox *)s;
	if (!a) return GF_OK;

	switch (a->type) {
	case GF_ISOM_BOX_TYPE_MP4S:
	case GF_ISOM_BOX_TYPE_ENCS:
	case GF_ISOM_BOX_TYPE_MP4A:
	case GF_ISOM_BOX_TYPE_ENCA:
	case GF_ISOM_BOX_TYPE_MP4V:
	case GF_ISOM_BOX_TYPE_ENCV:
	case GF_ISOM_BOX_TYPE_RESV:
	case GF_ISOM_BOX_TYPE_GHNT:
	case GF_ISOM_BOX_TYPE_RTP_STSD:
	case GF_ISOM_BOX_TYPE_SRTP_STSD:
	case GF_ISOM_BOX_TYPE_FDP_STSD:
	case GF_ISOM_BOX_TYPE_RRTP_STSD:
	case GF_ISOM_BOX_TYPE_RTCP_STSD:
	case GF_ISOM_BOX_TYPE_AVC1:
	case GF_ISOM_BOX_TYPE_AVC2:
	case GF_ISOM_BOX_TYPE_AVC3:
	case GF_ISOM_BOX_TYPE_AVC4:
	case GF_ISOM_BOX_TYPE_SVC1:
	case GF_ISOM_BOX_TYPE_MVC1:
	case GF_ISOM_BOX_TYPE_HVC1:
	case GF_ISOM_BOX_TYPE_HEV1:
	case GF_ISOM_BOX_TYPE_HVC2:
	case GF_ISOM_BOX_TYPE_HEV2:
	case GF_ISOM_BOX_TYPE_HVT1:
	case GF_ISOM_BOX_TYPE_LHV1:
	case GF_ISOM_BOX_TYPE_LHE1:
	case GF_ISOM_BOX_TYPE_TX3G:
	case GF_ISOM_BOX_TYPE_TEXT:
	case GF_ISOM_BOX_TYPE_ENCT:
	case GF_ISOM_BOX_TYPE_METX:
	case GF_ISOM_BOX_TYPE_METT:
	case GF_ISOM_BOX_TYPE_STXT:
	case GF_ISOM_BOX_TYPE_DIMS:
	case GF_ISOM_BOX_TYPE_AC3:
	case GF_ISOM_BOX_TYPE_EC3:
	case GF_ISOM_BOX_TYPE_LSR1:
	case GF_ISOM_BOX_TYPE_WVTT:
	case GF_ISOM_BOX_TYPE_STPP:
	case GF_ISOM_BOX_TYPE_SBTT:
	case GF_ISOM_BOX_TYPE_ELNG:
	case GF_ISOM_BOX_TYPE_MP3:
	case GF_ISOM_BOX_TYPE_JPEG:
	case GF_ISOM_BOX_TYPE_JP2K:
	case GF_ISOM_BOX_TYPE_PNG:
	case GF_ISOM_SUBTYPE_3GP_AMR:
	case GF_ISOM_SUBTYPE_3GP_AMR_WB:
	case GF_ISOM_SUBTYPE_3GP_EVRC:
	case GF_ISOM_SUBTYPE_3GP_QCELP:
	case GF_ISOM_SUBTYPE_3GP_SMV:
	case GF_ISOM_SUBTYPE_3GP_H263:
		return gf_isom_box_add_default((GF_Box*)ptr, a);

	case GF_ISOM_BOX_TYPE_UNKNOWN:
		def = (GF_UnknownBox *)a;
		/*we need at least 8 bytes for unknown sample entries*/
		if (def->dataSize < 8) {
			gf_isom_box_del(a);
			return GF_OK;
		}
		return gf_isom_box_add_default((GF_Box*)ptr, a);

	default:
		GF_LOG(GF_LOG_ERROR, GF_LOG_CONTAINER, ("[iso file] Cannot process box of type %s\n", gf_4cc_to_str(a->type)));
		return GF_ISOM_INVALID_FILE;
	}
}
