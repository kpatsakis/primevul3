static int hashKey(DIGEST_CTX hash, const struct pgpPkt *pkt, int exptag)
{
    int rc = -1;
    if (pkt->tag == exptag) {
	uint8_t head[] = {
	    0x99,
	    (pkt->blen >> 8),
	    (pkt->blen     ),
	};

	rpmDigestUpdate(hash, head, 3);
	rpmDigestUpdate(hash, pkt->body, pkt->blen);
	rc = 0;
    }
    return rc;
}