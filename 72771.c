mcs_send_dpu(unsigned short reason)
{
	STREAM s, contents;

	logger(Protocol, Debug, "mcs_send_dpu(), reason=%d", reason);

	contents = malloc(sizeof(struct stream));
	memset(contents, 0, sizeof(struct stream));
	s_realloc(contents, 6);
	s_reset(contents);
	ber_out_integer(contents, reason);	/* Reason */
	ber_out_sequence(contents, NULL);	/* SEQUENCE OF NonStandradParameters OPTIONAL */
	s_mark_end(contents);

	s = iso_init(8);
	ber_out_sequence(s, contents);
	s_free(contents);

	s_mark_end(s);

	iso_send(s);
}
