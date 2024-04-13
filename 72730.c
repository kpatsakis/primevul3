GF_StreamContext *gf_sm_stream_new(GF_SceneManager *ctx, u16 ES_ID, u8 streamType, u8 objectType)
{
	u32 i;
	GF_StreamContext *tmp;

	i=0;
	while ((tmp = (GF_StreamContext*)gf_list_enum(ctx->streams, &i))) {
		/*we MUST use the same ST*/
		if (tmp->streamType!=streamType) continue;
		/*if no ESID/OTI specified this is a base layer (default stream created by parsers)
		if ESID/OTI specified this is a stream already setup
		*/
		if ( tmp->ESID==ES_ID ) {
			return tmp;
		}
	}

	GF_SAFEALLOC(tmp, GF_StreamContext);
	if (!tmp) return NULL;
	tmp->AUs = gf_list_new();
	tmp->ESID = ES_ID;
	tmp->streamType = streamType;
	tmp->objectType = objectType ? objectType : 1;
	tmp->timeScale = 1000;
	gf_list_add(ctx->streams, tmp);
	return tmp;
}
