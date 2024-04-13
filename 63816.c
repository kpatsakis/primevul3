static void t1_putline(PDF pdf)
{
    char *p = t1_line_array;
    if (t1_line_ptr - t1_line_array <= 1)
        return;
    if (t1_eexec_encrypt) {
        while (p < t1_line_ptr)
            t1_putchar((eight_bits) eencrypt((byte) * p++));
    } else
        while (p < t1_line_ptr)
            t1_putchar((eight_bits) * p++);
}
