int pgpPrtParamsSubkeys(const uint8_t *pkts, size_t pktlen,
			pgpDigParams mainkey, pgpDigParams **subkeys,
			int *subkeysCount)
{
    const uint8_t *p = pkts;
    const uint8_t *pend = pkts + pktlen;
    pgpDigParams *digps = NULL;
    int count = 0;
    int alloced = 10;
    struct pgpPkt pkt;
    int rc, i;

    digps = xmalloc(alloced * sizeof(*digps));

    while (p < pend) {
	if (decodePkt(p, (pend - p), &pkt))
	    break;

	p += (pkt.body - pkt.head) + pkt.blen;

	if (pkt.tag == PGPTAG_PUBLIC_SUBKEY) {
	    if (count == alloced) {
		alloced <<= 1;
		digps = xrealloc(digps, alloced * sizeof(*digps));
	    }

	    digps[count] = pgpDigParamsNew(PGPTAG_PUBLIC_SUBKEY);
	    /* Copy UID from main key to subkey */
	    digps[count]->userid = xstrdup(mainkey->userid);

	    if (getKeyID(pkt.body, pkt.blen, digps[count]->signid)) {
		pgpDigParamsFree(digps[count]);
		continue;
	    }

	    if (pgpPrtKey(pkt.tag, pkt.body, pkt.blen, digps[count])) {
		pgpDigParamsFree(digps[count]);
		continue;
	    }
	    count++;
	}
    }
    rc = (p == pend) ? 0 : -1;

    if (rc == 0) {
	*subkeys = xrealloc(digps, count * sizeof(*digps));
	*subkeysCount = count;
    } else {
	for (i = 0; i < count; i++)
	    pgpDigParamsFree(digps[i]);
	free(digps);
    }

    return rc;
}