 void __sock_tx_timestamp(__u16 tsflags, __u8 *tx_flags)
{
	u8 flags = *tx_flags;

	if (tsflags & SOF_TIMESTAMPING_TX_HARDWARE)
		flags |= SKBTX_HW_TSTAMP;

	if (tsflags & SOF_TIMESTAMPING_TX_SOFTWARE)
		flags |= SKBTX_SW_TSTAMP;

	if (tsflags & SOF_TIMESTAMPING_TX_SCHED)
		flags |= SKBTX_SCHED_TSTAMP;

	*tx_flags = flags;
}
