mcs_send_aurq(void)
{
	STREAM s;
	logger(Protocol, Debug, "%s()", __func__);
	s = iso_init(1);

	out_uint8(s, (MCS_AURQ << 2));

	s_mark_end(s);
	iso_send(s);
}
