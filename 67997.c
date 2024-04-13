void AVC_RewriteESDescriptorEx(GF_MPEGVisualSampleEntryBox *avc, GF_MediaBox *mdia)
{
	GF_AVCConfig *avcc, *svcc, *mvcc;
	GF_BitRateBox *btrt = gf_isom_sample_entry_get_bitrate((GF_SampleEntryBox *)avc, GF_FALSE);

	if (avc->emul_esd) gf_odf_desc_del((GF_Descriptor *)avc->emul_esd);
	avc->emul_esd = gf_odf_desc_esd_new(2);
	avc->emul_esd->decoderConfig->streamType = GF_STREAM_VISUAL;
	/*AVC OTI is 0x21, AVC parameter set stream OTI (not supported in gpac) is 0x22, SVC OTI is 0x24*/
	/*if we have only SVC stream, set objectTypeIndication to AVC OTI; else set it to AVC OTI*/
	if (avc->svc_config && !avc->avc_config)
		avc->emul_esd->decoderConfig->objectTypeIndication = GPAC_OTI_VIDEO_SVC;
	else if (avc->mvc_config && !avc->avc_config)
		avc->emul_esd->decoderConfig->objectTypeIndication = GPAC_OTI_VIDEO_MVC;
	else
		avc->emul_esd->decoderConfig->objectTypeIndication = GPAC_OTI_VIDEO_AVC;

	if (btrt) {
		avc->emul_esd->decoderConfig->bufferSizeDB = btrt->bufferSizeDB;
		avc->emul_esd->decoderConfig->avgBitrate = btrt->avgBitrate;
		avc->emul_esd->decoderConfig->maxBitrate = btrt->maxBitrate;
	}
	if (avc->descr) {
		u32 i=0;
		GF_Descriptor *desc,*clone;
		i=0;
		while ((desc = (GF_Descriptor *)gf_list_enum(avc->descr->descriptors, &i))) {
			clone = NULL;
			gf_odf_desc_copy(desc, &clone);
			if (gf_odf_desc_add_desc((GF_Descriptor *)avc->emul_esd, clone) != GF_OK)
				gf_odf_desc_del(clone);
		}
	}
	if (avc->avc_config) {
		avcc = avc->avc_config->config ? AVC_DuplicateConfig(avc->avc_config->config) : NULL;
		/*merge SVC config*/
		if (avc->svc_config) {
			merge_avc_config(avcc, avc->svc_config->config);
		}
		/*merge MVC config*/
		if (avc->mvc_config) {
			merge_avc_config(avcc, avc->mvc_config->config);
		}
		if (avcc) {
			if (mdia) merge_all_config(avcc, NULL, mdia);

			gf_odf_avc_cfg_write(avcc, &avc->emul_esd->decoderConfig->decoderSpecificInfo->data, &avc->emul_esd->decoderConfig->decoderSpecificInfo->dataLength);
			gf_odf_avc_cfg_del(avcc);
		}
	} else if (avc->svc_config) {
		svcc = AVC_DuplicateConfig(avc->svc_config->config);

		if (mdia) merge_all_config(svcc, NULL, mdia);

		gf_odf_avc_cfg_write(svcc, &avc->emul_esd->decoderConfig->decoderSpecificInfo->data, &avc->emul_esd->decoderConfig->decoderSpecificInfo->dataLength);
		gf_odf_avc_cfg_del(svcc);
	}
	else if (avc->mvc_config) {
		mvcc = AVC_DuplicateConfig(avc->mvc_config->config);

		if (mdia) merge_all_config(mvcc, NULL, mdia);

		gf_odf_avc_cfg_write(mvcc, &avc->emul_esd->decoderConfig->decoderSpecificInfo->data, &avc->emul_esd->decoderConfig->decoderSpecificInfo->dataLength);
		gf_odf_avc_cfg_del(mvcc);
	}
}
