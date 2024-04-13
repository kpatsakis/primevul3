pgpArmor pgpReadPkts(const char * fn, uint8_t ** pkt, size_t * pktlen)
{
    uint8_t * b = NULL;
    ssize_t blen;
    pgpArmor ec = PGPARMOR_ERR_NO_BEGIN_PGP;	/* XXX assume failure */
    int rc = rpmioSlurp(fn, &b, &blen);
    if (rc == 0 && b != NULL && blen > 0) {
	ec = decodePkts(b, pkt, pktlen);
    }
    free(b);
    return ec;
}