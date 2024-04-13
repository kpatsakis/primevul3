static void t1_stop_eexec(void)
{
    int c;
    if (is_included(fm_cur)) {
        get_length2();
        save_offset();
    }
    end_last_eexec_line();
    if (!t1_pfa)
        t1_check_block_len(true);
    else {
        c = edecrypt((byte)t1_getbyte());
        if (!(c == 10 || c == 13)) {
            if (last_hexbyte == 0)
                t1_puts("00");
            else
                pdftex_warn("unexpected data after eexec");
        }
    }
    t1_cs = false;
    t1_in_eexec = 2;
}
