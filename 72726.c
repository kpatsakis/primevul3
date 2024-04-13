static void gf_sm_reset_stream(GF_StreamContext *sc)
{
	while (gf_list_count(sc->AUs)) {
		GF_AUContext *au = (GF_AUContext *)gf_list_last(sc->AUs);
		gf_list_rem_last(sc->AUs);
		gf_sm_au_del(sc, au);

	}
}
