size_t pktlen(unsigned char *buf)
{
	size_t hdr = sizeof(struct udphdr);

	return strlen((char *)buf + hdr) + hdr;
}
