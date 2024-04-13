static int pgpPrtKey(pgpTag tag, const uint8_t *h, size_t hlen,
		     pgpDigParams _digp)
{
    uint8_t version = 0;
    const uint8_t * p = NULL;
    int rc = 1;

    if (pgpVersion(h, hlen, &version))
	return rc;

    /* We only permit V4 keys, V3 keys are long long since deprecated */
    switch (version) {
    case 4:
    {   pgpPktKeyV4 v = (pgpPktKeyV4)h;

	if (hlen > sizeof(*v)) {
	    pgpPrtVal("V4 ", pgpTagTbl, tag);
	    pgpPrtVal(" ", pgpPubkeyTbl, v->pubkey_algo);
	    pgpPrtTime(" ", v->time, sizeof(v->time));
	    pgpPrtNL();

	    /* If _digp->hash is not NULL then signature is already loaded */
	    if (_digp->hash == NULL) {
		_digp->version = v->version;
		_digp->time = pgpGrab(v->time, sizeof(v->time));
		_digp->pubkey_algo = v->pubkey_algo;
	    }

	    p = ((uint8_t *)v) + sizeof(*v);
	    rc = pgpPrtPubkeyParams(v->pubkey_algo, p, h, hlen, _digp);
	}
    }	break;
    default:
	rpmlog(RPMLOG_WARNING, _("Unsupported version of key: V%d\n"), h[0]);
    }
    return rc;
}