static void gf_sm_au_del(GF_StreamContext *sc, GF_AUContext *au)
{
	while (gf_list_count(au->commands)) {
		void *comptr = gf_list_last(au->commands);
		gf_list_rem_last(au->commands);
		switch (sc->streamType) {
		case GF_STREAM_OD:
			gf_odf_com_del((GF_ODCom**) & comptr);
			break;
		case GF_STREAM_SCENE:
			gf_sg_command_del((GF_Command *)comptr);
			break;
		}
	}
	gf_list_del(au->commands);
	gf_free(au);
}
