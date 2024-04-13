static void t1_start_eexec(void)
{
    int i;
    if (is_included(fm_cur)) {
        get_length1();
        save_offset();
    }
    if (!t1_pfa)
        t1_check_block_len(false);
    for (t1_line_ptr = t1_line_array, i = 0; i < 4; i++) {
        edecrypt((byte)t1_getbyte());
        *t1_line_ptr++ = 0;
    }
    t1_eexec_encrypt = true;
    if (is_included(fm_cur))
        t1_putline(); /* to put the first four bytes */
}
