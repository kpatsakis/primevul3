static void gf_sm_delete_stream(GF_StreamContext *sc)
{
	gf_sm_reset_stream(sc);
	gf_list_del(sc->AUs);
	if (sc->name) gf_free(sc->name);
	if (sc->dec_cfg) gf_free(sc->dec_cfg);
	gf_free(sc);
}
