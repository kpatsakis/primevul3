GF_MuxInfo *gf_sm_get_mux_info(GF_ESD *src)
{
	u32 i;
	GF_MuxInfo *mux;
	i=0;
	while ((mux = (GF_MuxInfo *)gf_list_enum(src->extensionDescriptors, &i))) {
		if (mux->tag == GF_ODF_MUXINFO_TAG) return mux;
	}
	return NULL;
}
