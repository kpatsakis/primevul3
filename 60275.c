jpc_tsfb_t *jpc_cod_gettsfb(int qmfbid, int numlvls)
{
	jpc_tsfb_t *tsfb;

	if (!(tsfb = malloc(sizeof(jpc_tsfb_t))))
		return 0;

	if (numlvls > 0) {
		switch (qmfbid) {
		case JPC_COX_INS:
			tsfb->qmfb = &jpc_ns_qmfb2d;
			break;
		default:
		case JPC_COX_RFT:
			tsfb->qmfb = &jpc_ft_qmfb2d;
			break;
		}
	} else {
		tsfb->qmfb = 0;
	}
	tsfb->numlvls = numlvls;
	return tsfb;
}
