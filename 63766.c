static char *eol(char *s)
{
    char *p = strend(s);
    if (p - s > 1 && p[-1] != 10) {
        *p++ = 10;
        *p = 0;
    }
    return p;
}
