static void pdftex_warn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fputs("\nWarning: module writet1 of dvips", stderr);
    if (cur_file_name)
        fprintf(stderr, " (file %s)", cur_file_name);
    fputs(": ", stderr);
    vsprintf(print_buf, fmt, args);
    fputs(print_buf, stderr);
    fputs("\n", stderr);
    va_end(args);
}
