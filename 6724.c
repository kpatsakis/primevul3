static int pgpPrtSig(pgpTag tag, const uint8_t *h, size_t hlen,
		     pgpDigParams _digp)
{
    uint8_t version = 0;
    const uint8_t * p;
    unsigned int plen;
    int rc = 1;

    if (pgpVersion(h, hlen, &version))
	return rc;

    switch (version) {
    case 3:
    {   pgpPktSigV3 v = (pgpPktSigV3)h;

	if (hlen <= sizeof(*v) || v->hashlen != 5)
	    return 1;

	pgpPrtVal("V3 ", pgpTagTbl, tag);
	pgpPrtVal(" ", pgpPubkeyTbl, v->pubkey_algo);
	pgpPrtVal(" ", pgpHashTbl, v->hash_algo);
	pgpPrtVal(" ", pgpSigTypeTbl, v->sigtype);
	pgpPrtNL();
	pgpPrtTime(" ", v->time, sizeof(v->time));
	pgpPrtNL();
	pgpPrtHex(" signer keyid", v->signid, sizeof(v->signid));
	plen = pgpGrab(v->signhash16, sizeof(v->signhash16));
	pgpPrtHex(" signhash16", v->signhash16, sizeof(v->signhash16));
	pgpPrtNL();

	if (_digp->pubkey_algo == 0) {
	    _digp->version = v->version;
	    _digp->hashlen = v->hashlen;
	    _digp->sigtype = v->sigtype;
	    _digp->hash = memcpy(xmalloc(v->hashlen), &v->sigtype, v->hashlen);
	    _digp->time = pgpGrab(v->time, sizeof(v->time));
	    memcpy(_digp->signid, v->signid, sizeof(_digp->signid));
	    _digp->pubkey_algo = v->pubkey_algo;
	    _digp->hash_algo = v->hash_algo;
	    memcpy(_digp->signhash16, v->signhash16, sizeof(_digp->signhash16));
	}

	p = ((uint8_t *)v) + sizeof(*v);
	rc = pgpPrtSigParams(tag, v->pubkey_algo, p, h, hlen, _digp);
    }	break;
    case 4:
    {   pgpPktSigV4 v = (pgpPktSigV4)h;
	const uint8_t *const hend = h + hlen;

	if (hlen <= sizeof(*v))
	    return 1;

	pgpPrtVal("V4 ", pgpTagTbl, tag);
	pgpPrtVal(" ", pgpPubkeyTbl, v->pubkey_algo);
	pgpPrtVal(" ", pgpHashTbl, v->hash_algo);
	pgpPrtVal(" ", pgpSigTypeTbl, v->sigtype);
	pgpPrtNL();

	p = &v->hashlen[0];
	if (pgpGet(v->hashlen, sizeof(v->hashlen), hend, &plen))
	    return 1;
	p += sizeof(v->hashlen);

	if ((p + plen) > hend)
	    return 1;

	if (_digp->pubkey_algo == 0) {
	    _digp->hashlen = sizeof(*v) + plen;
	    _digp->hash = memcpy(xmalloc(_digp->hashlen), v, _digp->hashlen);
	}
	if (pgpPrtSubType(p, plen, v->sigtype, _digp))
	    return 1;
	p += plen;

	if (pgpGet(p, 2, hend, &plen))
	    return 1;
	p += 2;

	if ((p + plen) > hend)
	    return 1;

	if (pgpPrtSubType(p, plen, v->sigtype, _digp))
	    return 1;
	p += plen;

	if (h + hlen - p < 2)
	    return 1;
	pgpPrtHex(" signhash16", p, 2);
	pgpPrtNL();

	if (_digp->pubkey_algo == 0) {
	    _digp->version = v->version;
	    _digp->sigtype = v->sigtype;
	    _digp->pubkey_algo = v->pubkey_algo;
	    _digp->hash_algo = v->hash_algo;
	    memcpy(_digp->signhash16, p, sizeof(_digp->signhash16));
	}

	p += 2;
	if (p > hend)
	    return 1;

	rc = pgpPrtSigParams(tag, v->pubkey_algo, p, h, hlen, _digp);
    }	break;
    default:
	rpmlog(RPMLOG_WARNING, _("Unsupported version of signature: V%d\n"), version);
	rc = 1;
	break;
    }
    return rc;
}