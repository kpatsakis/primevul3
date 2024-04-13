GF_Err AVC_HEVC_UpdateESD(GF_MPEGVisualSampleEntryBox *avc, GF_ESD *esd)
{
	GF_BitRateBox *btrt = gf_isom_sample_entry_get_bitrate((GF_SampleEntryBox *)avc, GF_TRUE);

	if (avc->descr) gf_isom_box_del((GF_Box *) avc->descr);
	avc->descr = NULL;
	btrt->avgBitrate = esd->decoderConfig->avgBitrate;
	btrt->maxBitrate = esd->decoderConfig->maxBitrate;
	btrt->bufferSizeDB = esd->decoderConfig->bufferSizeDB;

	if (gf_list_count(esd->IPIDataSet)
	        || gf_list_count(esd->IPMPDescriptorPointers)
	        || esd->langDesc
	        || gf_list_count(esd->extensionDescriptors)
	        || esd->ipiPtr || esd->qos || esd->RegDescriptor) {

		avc->descr = (GF_MPEG4ExtensionDescriptorsBox *)gf_isom_box_new(GF_ISOM_BOX_TYPE_M4DS);
		if (esd->RegDescriptor) {
			gf_list_add(avc->descr->descriptors, esd->RegDescriptor);
			esd->RegDescriptor = NULL;
		}
		if (esd->qos) {
			gf_list_add(avc->descr->descriptors, esd->qos);
			esd->qos = NULL;
		}
		if (esd->ipiPtr) {
			gf_list_add(avc->descr->descriptors, esd->ipiPtr);
			esd->ipiPtr= NULL;
		}

		while (gf_list_count(esd->IPIDataSet)) {
			GF_Descriptor *desc = (GF_Descriptor *)gf_list_get(esd->IPIDataSet, 0);
			gf_list_rem(esd->IPIDataSet, 0);
			gf_list_add(avc->descr->descriptors, desc);
		}
		while (gf_list_count(esd->IPMPDescriptorPointers)) {
			GF_Descriptor *desc = (GF_Descriptor *)gf_list_get(esd->IPMPDescriptorPointers, 0);
			gf_list_rem(esd->IPMPDescriptorPointers, 0);
			gf_list_add(avc->descr->descriptors, desc);
		}
		if (esd->langDesc) {
			gf_list_add(avc->descr->descriptors, esd->langDesc);
			esd->langDesc = NULL;
		}
		while (gf_list_count(esd->extensionDescriptors)) {
			GF_Descriptor *desc = (GF_Descriptor *)gf_list_get(esd->extensionDescriptors, 0);
			gf_list_rem(esd->extensionDescriptors, 0);
			gf_list_add(avc->descr->descriptors, desc);
		}
	}


	if (!avc->lhvc_config && (esd->decoderConfig->objectTypeIndication==GPAC_OTI_VIDEO_HEVC)) {
		if (!avc->hevc_config) avc->hevc_config = (GF_HEVCConfigurationBox *)gf_isom_box_new(GF_ISOM_BOX_TYPE_HVCC);
		if (esd->decoderConfig->decoderSpecificInfo && esd->decoderConfig->decoderSpecificInfo->data) {
			if (avc->hevc_config->config) gf_odf_hevc_cfg_del(avc->hevc_config->config);
			avc->hevc_config->config = gf_odf_hevc_cfg_read(esd->decoderConfig->decoderSpecificInfo->data, esd->decoderConfig->decoderSpecificInfo->dataLength, GF_FALSE);
		}
	}
	else if (!avc->svc_config && !avc->mvc_config && (esd->decoderConfig->objectTypeIndication==GPAC_OTI_VIDEO_AVC)) {
		if (!avc->avc_config) avc->avc_config = (GF_AVCConfigurationBox *)gf_isom_box_new(GF_ISOM_BOX_TYPE_AVCC);
		if (esd->decoderConfig->decoderSpecificInfo && esd->decoderConfig->decoderSpecificInfo->data) {
			if (avc->avc_config->config) gf_odf_avc_cfg_del(avc->avc_config->config);
			avc->avc_config->config = gf_odf_avc_cfg_read(esd->decoderConfig->decoderSpecificInfo->data, esd->decoderConfig->decoderSpecificInfo->dataLength);
		}
	}

	gf_odf_desc_del((GF_Descriptor *)esd);
	if (avc->hevc_config) {
		HEVC_RewriteESDescriptor(avc);
	} else {
		AVC_RewriteESDescriptor(avc);
	}
	return GF_OK;
}
