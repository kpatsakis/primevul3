int pgpPubKeyCertLen(const uint8_t *pkts, size_t pktslen, size_t *certlen)
{
    const uint8_t *p = pkts;
    const uint8_t *pend = pkts + pktslen;
    struct pgpPkt pkt;

    while (p < pend) {
	if (decodePkt(p, (pend - p), &pkt))
	    return -1;

	if (pkt.tag == PGPTAG_PUBLIC_KEY && pkts != p) {
	    *certlen = p - pkts;
	    return 0;
	}

	p += (pkt.body - pkt.head) + pkt.blen;
    }

    *certlen = pktslen;

    return 0;
}