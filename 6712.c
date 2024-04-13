char * pgpHexStr(const uint8_t *p, size_t plen)
{
    char *t, *str;
    str = t = xmalloc(plen * 2 + 1);
    static char const hex[] = "0123456789abcdef";
    while (plen-- > 0) {
	size_t i;
	i = *p++;
	*t++ = hex[ (i >> 4) & 0xf ];
	*t++ = hex[ (i     ) & 0xf ];
    }
    *t = '\0';
    return str;
}