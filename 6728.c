int pgpPubkeyFingerprint(const uint8_t *h, size_t hlen,
			  uint8_t **fp, size_t *fplen)
{
    int rc = -1; /* assume failure */
    const uint8_t *se;
    const uint8_t *pend = h + hlen;
    uint8_t version = 0;

    if (pgpVersion(h, hlen, &version))
	return rc;

    /* We only permit V4 keys, V3 keys are long long since deprecated */
    switch (version) {
    case 4:
      {	pgpPktKeyV4 v = (pgpPktKeyV4) (h);
	int mpis = -1;

	/* Packet must be larger than v to have room for the required MPIs */
	if (hlen > sizeof(*v)) {
	    switch (v->pubkey_algo) {
	    case PGPPUBKEYALGO_RSA:
		mpis = 2;
		break;
	    case PGPPUBKEYALGO_DSA:
		mpis = 4;
		break;
	    case PGPPUBKEYALGO_EDDSA:
		mpis = 1;
		break;
	    }
	}

	se = (uint8_t *)(v + 1);
	/* EdDSA has a curve id before the MPIs */
	if (v->pubkey_algo == PGPPUBKEYALGO_EDDSA) {
	    if (se < pend && se[0] != 0x00 && se[0] != 0xff)
		se += 1 + se[0];
	    else
		se = pend;      /* error out when reading the MPI */
	}
	while (se < pend && mpis-- > 0)
	    se += pgpMpiLen(se);

	/* Does the size and number of MPI's match our expectations? */
	if (se == pend && mpis == 0) {
	    DIGEST_CTX ctx = rpmDigestInit(PGPHASHALGO_SHA1, RPMDIGEST_NONE);
	    uint8_t *d = NULL;
	    size_t dlen = 0;
	    int i = se - h;
	    uint8_t in[3] = { 0x99, (i >> 8), i };

	    (void) rpmDigestUpdate(ctx, in, 3);
	    (void) rpmDigestUpdate(ctx, h, i);
	    (void) rpmDigestFinal(ctx, (void **)&d, &dlen, 0);

	    if (dlen == 20) {
		rc = 0;
		*fp = d;
		*fplen = dlen;
	    } else {
		free(d);
	    }
	}

      }	break;
    default:
	rpmlog(RPMLOG_WARNING, _("Unsupported version of key: V%d\n"), version);
    }
    return rc;
}