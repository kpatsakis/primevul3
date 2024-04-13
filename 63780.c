static void t1_putline(void)
{
    char *p = t1_line_array;
    if (t1_line_ptr - t1_line_array <= 1)
        return;
    if (t1_eexec_encrypt) {
        while (p < t1_line_ptr)
            t1_outhex(eencrypt(*p++)); /* dvips outputs hex, unlike pdftex */
    } else
        while (p < t1_line_ptr)
            t1_putchar(*p++);
}
