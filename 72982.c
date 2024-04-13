tstamp_precision_to_string(int precision)
{
	switch (precision) {

	case PCAP_TSTAMP_PRECISION_MICRO:
		return "micro";

	case PCAP_TSTAMP_PRECISION_NANO:
		return "nano";

	default:
		return "unknown";
	}
}
