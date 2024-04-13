void gf_sm_stream_del(GF_SceneManager *ctx, GF_StreamContext *sc)
{
	if (gf_list_del_item(ctx->streams, sc)>=0) {
		gf_sm_delete_stream(sc);
	}
}
