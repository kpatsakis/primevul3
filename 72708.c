static GF_Descriptor *FFD_GetServiceDesc(GF_InputService *plug, u32 expect_type, const char *sub_url)
{
	GF_ObjectDescriptor *od;
	GF_ESD *esd;
	FFDemux *ffd = (FFDemux*)plug->priv;

	if (!ffd->ctx) return NULL;

	if (expect_type==GF_MEDIA_OBJECT_UNDEF) {
		if (ffd->video_st>=0) expect_type=GF_MEDIA_OBJECT_VIDEO;
		else if (ffd->audio_st>=0) expect_type=GF_MEDIA_OBJECT_AUDIO;
	}


	/*since we don't handle multitrack in ffmpeg, we don't need to check sub_url, only use expected type*/
	if (expect_type==GF_MEDIA_OBJECT_AUDIO) {
		if (ffd->audio_st<0) return NULL;
		od = (GF_ObjectDescriptor *) gf_odf_desc_new(GF_ODF_OD_TAG);
		od->objectDescriptorID = 1;
		esd = FFD_GetESDescriptor(ffd, GF_TRUE);
		/*if session join, setup sync*/
		if (ffd->video_ch) esd->OCRESID = ffd->video_st+1;
		gf_list_add(od->ESDescriptors, esd);
		ffd->service_type = 2;
		return (GF_Descriptor *) od;
	}
	if (expect_type==GF_MEDIA_OBJECT_VIDEO) {
		if (ffd->video_st<0) return NULL;
		od = (GF_ObjectDescriptor *) gf_odf_desc_new(GF_ODF_OD_TAG);
		od->objectDescriptorID = 1;
		esd = FFD_GetESDescriptor(ffd, GF_FALSE);
		/*if session join, setup sync*/
		if (ffd->audio_ch) esd->OCRESID = ffd->audio_st+1;
		gf_list_add(od->ESDescriptors, esd);
		ffd->service_type = 1;
		return (GF_Descriptor *) od;
	}
	return NULL;
}
