static void http_av_log(void *ptr, int level, const char *fmt, va_list vargs)
{
    static int print_prefix = 1;
    AVClass *avc = ptr ? *(AVClass**)ptr : NULL;
    if (level > av_log_get_level())
        return;
    if (print_prefix && avc)
        http_log("[%s @ %p]", avc->item_name(ptr), ptr);
    print_prefix = strstr(fmt, "\n") != NULL;
    http_vlog(fmt, vargs);
}
