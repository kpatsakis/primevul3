pgpDigParams pgpDigGetParams(pgpDig dig, unsigned int pkttype)
{
    pgpDigParams params = NULL;
    if (dig) {
	switch (pkttype) {
	case PGPTAG_SIGNATURE:
	    params = dig->signature;
	    break;
	case PGPTAG_PUBLIC_KEY:
	    params = dig->pubkey;
	    break;
	}
    }
    return params;
}