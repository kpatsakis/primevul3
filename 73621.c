static int cx24116_read_snr(struct dvb_frontend *fe, u16 *snr)
{
	if (esno_snr == 1)
		return cx24116_read_snr_esno(fe, snr);
	else
		return cx24116_read_snr_pct(fe, snr);
}
