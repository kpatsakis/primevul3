void gf_sm_reset(GF_SceneManager *ctx)
{
	GF_StreamContext *sc;
	u32 i=0;
	while ( (sc = gf_list_enum(ctx->streams, &i)) ) {
		gf_sm_reset_stream(sc);
	}
	if (ctx->root_od) gf_odf_desc_del((GF_Descriptor *) ctx->root_od);
	ctx->root_od = NULL;
}
