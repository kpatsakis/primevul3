static void t1_stop_eexec(PDF pdf)
{
    int c;
    get_length2();
    save_offset();
    t1_eexec_encrypt = false;
    if (!t1_pfa)
        t1_check_block_len(true);
    else {
        c = edecrypt((byte) t1_getbyte());
        if (!(c == 10 || c == 13)) {
            if (last_hexbyte == 0)
                t1_puts(pdf, "00");
            else
                normal_error("type 1","unexpected data after eexec");
        }
    }
    t1_cs = false;
    t1_in_eexec = 2;
}
