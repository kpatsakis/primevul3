static void t1_puts(const char *s)
{
    if (s != t1_line_array)
        strcpy(t1_line_array, s);
    t1_line_ptr = strend(t1_line_array);
    t1_putline();
}
