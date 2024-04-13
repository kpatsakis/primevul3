static void http_vlog(const char *fmt, va_list vargs)
{
    static int print_prefix = 1;
    char buf[32];

    if (!logfile)
        return;

    if (print_prefix) {
        ctime1(buf, sizeof(buf));
        fprintf(logfile, "%s ", buf);
    }
    print_prefix = strstr(fmt, "\n") != NULL;
    vfprintf(logfile, fmt, vargs);
    fflush(logfile);
}
