static GF_StreamContext *gf_sm_get_stream(GF_SceneManager *ctx, u16 ESID)
{
	u32 i, count;
	count = gf_list_count(ctx->streams);
	for (i=0; i<count; i++) {
		GF_StreamContext *sc = gf_list_get(ctx->streams, i);
		if (sc->ESID==ESID) return sc;
	}
	return NULL;
}
