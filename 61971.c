void jpc_enc_cp_destroy(jpc_enc_cp_t *cp)
{
	if (cp->ccps) {
		if (cp->tcp.ilyrrates) {
			jas_free(cp->tcp.ilyrrates);
		}
		jas_free(cp->ccps);
	}
	jas_free(cp);
}
