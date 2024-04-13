static void cs_fail(const char *cs_name, int subr, const char *fmt, ...)
{
    char buf[SMALL_BUF_SIZE];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    if (cs_name == NULL)
        formatted_error("type 1","Subr '%i': %s", (int) subr, buf);
    else
        formatted_error("type 1","CharString (/%s): %s", cs_name, buf);
}
