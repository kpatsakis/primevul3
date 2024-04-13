static void cs_fail(const char *cs_name, int subr, const char *fmt, ...)
{
    char buf[SMALL_BUF_SIZE];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    if (cs_name == NULL)
        pdftex_fail("Subr (%i): %s", (int) subr, buf);
    else
        pdftex_fail("CharString (/%s): %s", cs_name, buf);
}
