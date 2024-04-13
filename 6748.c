static int pgpPrtSubType(const uint8_t *h, size_t hlen, pgpSigType sigtype, 
			 pgpDigParams _digp)
{
    const uint8_t *p = h;
    size_t plen = 0, i;
    int rc = 0;

    while (hlen > 0 && rc == 0) {
	int impl = 0;
	i = pgpLen(p, hlen, &plen);
	if (i == 0 || plen < 1 || i + plen > hlen)
	    break;

	p += i;
	hlen -= i;

	pgpPrtVal("    ", pgpSubTypeTbl, (p[0]&(~PGPSUBTYPE_CRITICAL)));
	if (p[0] & PGPSUBTYPE_CRITICAL)
	    if (_print)
		fprintf(stderr, " *CRITICAL*");
	switch (*p) {
	case PGPSUBTYPE_PREFER_SYMKEY:	/* preferred symmetric algorithms */
	    for (i = 1; i < plen; i++)
		pgpPrtVal(" ", pgpSymkeyTbl, p[i]);
	    break;
	case PGPSUBTYPE_PREFER_HASH:	/* preferred hash algorithms */
	    for (i = 1; i < plen; i++)
		pgpPrtVal(" ", pgpHashTbl, p[i]);
	    break;
	case PGPSUBTYPE_PREFER_COMPRESS:/* preferred compression algorithms */
	    for (i = 1; i < plen; i++)
		pgpPrtVal(" ", pgpCompressionTbl, p[i]);
	    break;
	case PGPSUBTYPE_KEYSERVER_PREFERS:/* key server preferences */
	    for (i = 1; i < plen; i++)
		pgpPrtVal(" ", pgpKeyServerPrefsTbl, p[i]);
	    break;
	case PGPSUBTYPE_SIG_CREATE_TIME:
	    impl = *p;
	    if (!(_digp->saved & PGPDIG_SAVED_TIME) &&
		(sigtype == PGPSIGTYPE_POSITIVE_CERT || sigtype == PGPSIGTYPE_BINARY || sigtype == PGPSIGTYPE_TEXT || sigtype == PGPSIGTYPE_STANDALONE))
	    {
		if (plen-1 != sizeof(_digp->time))
		    break;
		_digp->saved |= PGPDIG_SAVED_TIME;
		_digp->time = pgpGrab(p+1, sizeof(_digp->time));
	    }
	case PGPSUBTYPE_SIG_EXPIRE_TIME:
	case PGPSUBTYPE_KEY_EXPIRE_TIME:
	    pgpPrtTime(" ", p+1, plen-1);
	    break;

	case PGPSUBTYPE_ISSUER_KEYID:	/* issuer key ID */
	    impl = *p;
	    if (!(_digp->saved & PGPDIG_SAVED_ID) &&
		(sigtype == PGPSIGTYPE_POSITIVE_CERT || sigtype == PGPSIGTYPE_BINARY || sigtype == PGPSIGTYPE_TEXT || sigtype == PGPSIGTYPE_STANDALONE))
	    {
		if (plen-1 != sizeof(_digp->signid))
		    break;
		_digp->saved |= PGPDIG_SAVED_ID;
		memcpy(_digp->signid, p+1, sizeof(_digp->signid));
	    }
	case PGPSUBTYPE_EXPORTABLE_CERT:
	case PGPSUBTYPE_TRUST_SIG:
	case PGPSUBTYPE_REGEX:
	case PGPSUBTYPE_REVOCABLE:
	case PGPSUBTYPE_ARR:
	case PGPSUBTYPE_REVOKE_KEY:
	case PGPSUBTYPE_NOTATION:
	case PGPSUBTYPE_PREFER_KEYSERVER:
	case PGPSUBTYPE_PRIMARY_USERID:
	case PGPSUBTYPE_POLICY_URL:
	case PGPSUBTYPE_KEY_FLAGS:
	case PGPSUBTYPE_SIGNER_USERID:
	case PGPSUBTYPE_REVOKE_REASON:
	case PGPSUBTYPE_FEATURES:
	case PGPSUBTYPE_EMBEDDED_SIG:
	case PGPSUBTYPE_INTERNAL_100:
	case PGPSUBTYPE_INTERNAL_101:
	case PGPSUBTYPE_INTERNAL_102:
	case PGPSUBTYPE_INTERNAL_103:
	case PGPSUBTYPE_INTERNAL_104:
	case PGPSUBTYPE_INTERNAL_105:
	case PGPSUBTYPE_INTERNAL_106:
	case PGPSUBTYPE_INTERNAL_107:
	case PGPSUBTYPE_INTERNAL_108:
	case PGPSUBTYPE_INTERNAL_109:
	case PGPSUBTYPE_INTERNAL_110:
	default:
	    pgpPrtHex("", p+1, plen-1);
	    break;
	}
	pgpPrtNL();

	if (!impl && (p[0] & PGPSUBTYPE_CRITICAL))
	    rc = 1;

	p += plen;
	hlen -= plen;
    }

    if (hlen != 0)
	rc = 1;

    return rc;
}