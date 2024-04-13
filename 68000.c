void HEVC_RewriteESDescriptorEx(GF_MPEGVisualSampleEntryBox *hevc, GF_MediaBox *mdia)
{
	GF_BitRateBox *btrt = gf_isom_sample_entry_get_bitrate((GF_SampleEntryBox *)hevc, GF_FALSE);

	if (hevc->emul_esd) gf_odf_desc_del((GF_Descriptor *)hevc->emul_esd);
	hevc->emul_esd = gf_odf_desc_esd_new(2);
	hevc->emul_esd->decoderConfig->streamType = GF_STREAM_VISUAL;
	hevc->emul_esd->decoderConfig->objectTypeIndication = GPAC_OTI_VIDEO_HEVC;
	if (hevc->lhvc_config /*&& !hevc->hevc_config*/)
		hevc->emul_esd->decoderConfig->objectTypeIndication = GPAC_OTI_VIDEO_LHVC;

	if (btrt) {
		hevc->emul_esd->decoderConfig->bufferSizeDB = btrt->bufferSizeDB;
		hevc->emul_esd->decoderConfig->avgBitrate = btrt->avgBitrate;
		hevc->emul_esd->decoderConfig->maxBitrate = btrt->maxBitrate;
	}
	if (hevc->descr) {
		u32 i=0;
		GF_Descriptor *desc,*clone;
		i=0;
		while ((desc = (GF_Descriptor *)gf_list_enum(hevc->descr->descriptors, &i))) {
			clone = NULL;
			gf_odf_desc_copy(desc, &clone);
			if (gf_odf_desc_add_desc((GF_Descriptor *)hevc->emul_esd, clone) != GF_OK)
				gf_odf_desc_del(clone);
		}
	}

	if (hevc->hevc_config || hevc->lhvc_config) {
		GF_HEVCConfig *hcfg = HEVC_DuplicateConfig(hevc->hevc_config ? hevc->hevc_config->config : hevc->lhvc_config->config);

		if (hevc->hevc_config && hevc->lhvc_config) {
			merge_hevc_config(hcfg, hevc->lhvc_config->config, GF_FALSE);
		}

		if (mdia) merge_all_config(NULL, hcfg, mdia);

		if (hcfg) {
			if (mdia && ((mdia->mediaTrack->extractor_mode&0x0000FFFF) != GF_ISOM_NALU_EXTRACT_INSPECT)) {
				hcfg->is_lhvc=GF_FALSE;
			}
			gf_odf_hevc_cfg_write(hcfg, &hevc->emul_esd->decoderConfig->decoderSpecificInfo->data, &hevc->emul_esd->decoderConfig->decoderSpecificInfo->dataLength);
			gf_odf_hevc_cfg_del(hcfg);
		}
	}
}
