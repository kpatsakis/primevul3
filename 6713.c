static int getKeyID(const uint8_t *h, size_t hlen, pgpKeyID_t keyid)
{
    uint8_t *fp = NULL;
    size_t fplen = 0;
    int rc = pgpPubkeyFingerprint(h, hlen, &fp, &fplen);
    if (fp && fplen > 8) {
	memcpy(keyid, (fp + (fplen-8)), 8);
	free(fp);
    }
    return rc;
}