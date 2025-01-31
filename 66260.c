GF_Err video_sample_entry_AddBox(GF_Box *s, GF_Box *a)
{
	GF_MPEGVisualSampleEntryBox *ptr = (GF_MPEGVisualSampleEntryBox *)s;
	switch (a->type) {
	case GF_ISOM_BOX_TYPE_ESDS:
		if (ptr->esd) ERROR_ON_DUPLICATED_BOX(a, ptr)
			ptr->esd = (GF_ESDBox *)a;
		break;
	case GF_ISOM_BOX_TYPE_SINF:
		gf_list_add(ptr->protections, a);
		break;
	case GF_ISOM_BOX_TYPE_RINF:
		if (ptr->rinf) ERROR_ON_DUPLICATED_BOX(a, ptr)
		ptr->rinf = (GF_RestrictedSchemeInfoBox *) a;
		break;
	case GF_ISOM_BOX_TYPE_AVCC:
		if (ptr->avc_config) ERROR_ON_DUPLICATED_BOX(a, ptr)
			ptr->avc_config = (GF_AVCConfigurationBox *)a;
		break;
	case GF_ISOM_BOX_TYPE_HVCC:
		if (ptr->hevc_config) ERROR_ON_DUPLICATED_BOX(a, ptr)
			ptr->hevc_config = (GF_HEVCConfigurationBox *)a;
		break;
	case GF_ISOM_BOX_TYPE_SVCC:
		if (ptr->svc_config) ERROR_ON_DUPLICATED_BOX(a, ptr)
			ptr->svc_config = (GF_AVCConfigurationBox *)a;
		break;
	case GF_ISOM_BOX_TYPE_MVCC:
		if (ptr->mvc_config) ERROR_ON_DUPLICATED_BOX(a, ptr)
			ptr->mvc_config = (GF_AVCConfigurationBox *)a;
		break;
	case GF_ISOM_BOX_TYPE_LHVC:
		if (ptr->lhvc_config) ERROR_ON_DUPLICATED_BOX(a, ptr)
			ptr->lhvc_config = (GF_HEVCConfigurationBox *)a;
		break;
	case GF_ISOM_BOX_TYPE_M4DS:
		if (ptr->descr) ERROR_ON_DUPLICATED_BOX(a, ptr)
			ptr->descr = (GF_MPEG4ExtensionDescriptorsBox *)a;
		break;
	case GF_ISOM_BOX_TYPE_UUID:
		if (! memcmp(((GF_UnknownUUIDBox*)a)->uuid, GF_ISOM_IPOD_EXT, 16)) {
			if (ptr->ipod_ext) ERROR_ON_DUPLICATED_BOX(a, ptr)
			ptr->ipod_ext = (GF_UnknownUUIDBox *)a;
		} else {
			return gf_isom_box_add_default(s, a);
		}
		break;
	case GF_ISOM_BOX_TYPE_D263:
		ptr->cfg_3gpp = (GF_3GPPConfigBox *)a;
		/*for 3GP config, remember sample entry type in config*/
		ptr->cfg_3gpp->cfg.type = ptr->type;
		break;
		break;

	case GF_ISOM_BOX_TYPE_PASP:
		if (ptr->pasp) ERROR_ON_DUPLICATED_BOX(a, ptr)
			ptr->pasp = (GF_PixelAspectRatioBox *)a;
		break;
	case GF_ISOM_BOX_TYPE_CLAP:
		if (ptr->clap) ERROR_ON_DUPLICATED_BOX(a, ptr)
			ptr->clap = (GF_CleanAppertureBox *)a;
		break;
	case GF_ISOM_BOX_TYPE_RVCC:
		if (ptr->rvcc) ERROR_ON_DUPLICATED_BOX(a, ptr)
			ptr->rvcc = (GF_RVCConfigurationBox *)a;
		break;
	default:
		return gf_isom_box_add_default(s, a);
	}
	return GF_OK;
}
