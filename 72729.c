GF_StreamContext *gf_sm_stream_find(GF_SceneManager *ctx, u16 ES_ID)
{
	u32 i, count;
	if (!ES_ID) return NULL;
	count = gf_list_count(ctx->streams);
	for (i=0; i<count; i++) {
		GF_StreamContext *tmp = (GF_StreamContext *)gf_list_get(ctx->streams, i);
		if (tmp->ESID==ES_ID) return tmp;
	}
	return NULL;
}
