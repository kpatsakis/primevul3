int pgpPrtPkts(const uint8_t * pkts, size_t pktlen, pgpDig dig, int printing)
{
    int rc;
    pgpDigParams digp = NULL;

    _print = printing;

    rc = pgpPrtParams(pkts, pktlen, 0, &digp);

    if (dig && rc == 0) {
	if (digp->tag == PGPTAG_SIGNATURE) {
	    pgpDigParamsFree(dig->signature);
	    dig->signature = digp;
	} else {
	    pgpDigParamsFree(dig->pubkey);
	    dig->pubkey = digp;
	}
    } else {
	pgpDigParamsFree(digp);
    }

    return rc;
}