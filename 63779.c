static void t1_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(t1_line_array, fmt, args);
    t1_puts(t1_line_array);
    va_end(args);
}
