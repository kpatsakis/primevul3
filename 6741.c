static int pgpVersion(const uint8_t *h, size_t hlen, uint8_t *version)
{
    if (hlen < 1)
	return -1;

    *version = h[0];
    return 0;
}