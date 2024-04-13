static void t1_start_eexec(PDF pdf)
{
    int i;
    get_length1();
    save_offset();
    if (!t1_pfa)
        t1_check_block_len(false);
    for (t1_line_ptr = t1_line_array, i = 0; i < 4; i++) {
        edecrypt((byte) t1_getbyte());
        *t1_line_ptr++ = 0;
    }
    t1_eexec_encrypt = true;
    /*tex To put the first four bytes: */
    t1_putline(pdf);
}
