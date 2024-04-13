static void pgpPrtHex(const char *pre, const uint8_t *p, size_t plen)
{
    char *hex = NULL;
    if (!_print) return;
    if (pre && *pre)
	fprintf(stderr, "%s", pre);
    hex = pgpHexStr(p, plen);
    fprintf(stderr, " %s", hex);
    free(hex);
}