static int pgpPrtPubkeyParams(uint8_t pubkey_algo,
		const uint8_t *p, const uint8_t *h, size_t hlen,
		pgpDigParams keyp)
{
    int rc = 1;
    const uint8_t *pend = h + hlen;
    int i, curve = 0;
    pgpDigAlg keyalg;
    if (pubkey_algo == PGPPUBKEYALGO_EDDSA) {
	int len = p + 1 < pend ? p[0] : 0;
	if (len == 0 || len == 0xff || p + 1 + len > pend)
	    goto exit;
	curve = pgpCurveByOid(p + 1, len);
	p += len + 1;
    }
    keyalg = pgpPubkeyNew(pubkey_algo, curve);
    for (i = 0; i < keyalg->mpis && p + 2 <= pend; i++) {
	int mpil = pgpMpiLen(p);
	if (p + mpil > pend)
	    break;
	if (keyalg->setmpi(keyalg, i, p))
	    break;
	p += mpil;
    }

    /* Does the size and number of MPI's match our expectations? */
    if (p == pend && i == keyalg->mpis)
	rc = 0;

    /* We can't handle more than one key at a time */
    if (rc == 0 && keyp->alg == NULL && (keyp->tag == PGPTAG_PUBLIC_KEY ||
	keyp->tag == PGPTAG_PUBLIC_SUBKEY))

	keyp->alg = keyalg;
    else
	pgpDigAlgFree(keyalg);

exit:
    return rc;
}