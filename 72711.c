static void FFD_SetupObjects(FFDemux *ffd)
{
	GF_ESD *esd;
	GF_ObjectDescriptor *od;
	u32 audio_esid = 0;

	if ((ffd->audio_st>=0) && (ffd->service_type != 1)) {
		od = (GF_ObjectDescriptor *) gf_odf_desc_new(GF_ODF_OD_TAG);
		esd = FFD_GetESDescriptor(ffd, GF_TRUE);
		od->objectDescriptorID = esd->ESID;
		audio_esid = esd->ESID;
		gf_list_add(od->ESDescriptors, esd);
		gf_service_declare_media(ffd->service, (GF_Descriptor*)od, (ffd->video_st>=0) ? GF_TRUE : GF_FALSE);
	}
	if ((ffd->video_st>=0) && (ffd->service_type != 2)) {
		od = (GF_ObjectDescriptor *) gf_odf_desc_new(GF_ODF_OD_TAG);
		esd = FFD_GetESDescriptor(ffd, GF_FALSE);
		od->objectDescriptorID = esd->ESID;
		esd->OCRESID = audio_esid;
		gf_list_add(od->ESDescriptors, esd);
		gf_service_declare_media(ffd->service, (GF_Descriptor*)od, GF_FALSE);
	}
}
