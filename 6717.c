static int pgpPrtSigParams(pgpTag tag, uint8_t pubkey_algo,
		const uint8_t *p, const uint8_t *h, size_t hlen,
		pgpDigParams sigp)
{
    int rc = 1; /* assume failure */
    const uint8_t * pend = h + hlen;
    int i;
    pgpDigAlg sigalg = pgpSignatureNew(pubkey_algo);

    for (i = 0; i < sigalg->mpis && pend - p >= 2; i++) {
	int mpil = pgpMpiLen(p);
	if (pend - p < mpil)
	    break;
	if (sigalg->setmpi(sigalg, i, p))
	    break;
	p += mpil;
    }

    /* Does the size and number of MPI's match our expectations? */
    if (p == pend && i == sigalg->mpis)
	rc = 0;
    
    /* We can't handle more than one sig at a time */
    if (rc == 0 && sigp->alg == NULL && sigp->tag == PGPTAG_SIGNATURE)
	sigp->alg = sigalg;
    else
	pgpDigAlgFree(sigalg);

    return rc;
}