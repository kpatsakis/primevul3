tstamp_precision_from_string(const char *precision)
{
	if (strncmp(precision, "nano", strlen("nano")) == 0)
		return PCAP_TSTAMP_PRECISION_NANO;

	if (strncmp(precision, "micro", strlen("micro")) == 0)
		return PCAP_TSTAMP_PRECISION_MICRO;

	return -EINVAL;
}
