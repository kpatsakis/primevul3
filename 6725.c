static int pgpVerifySelf(pgpDigParams key, pgpDigParams selfsig,
			const struct pgpPkt *all, int i)
{
    int rc = -1;
    DIGEST_CTX hash = NULL;

    switch (selfsig->sigtype) {
    case PGPSIGTYPE_SUBKEY_BINDING:
	hash = rpmDigestInit(selfsig->hash_algo, 0);
	if (hash) {
	    rc = hashKey(hash, &all[0], PGPTAG_PUBLIC_KEY);
	    if (!rc)
		rc = hashKey(hash, &all[i-1], PGPTAG_PUBLIC_SUBKEY);
	}
	break;
    default:
	/* ignore types we can't handle */
	rc = 0;
	break;
    }

    if (hash && rc == 0)
	rc = pgpVerifySignature(key, selfsig, hash);

    rpmDigestFinal(hash, NULL, NULL, 0);

    return rc;
}