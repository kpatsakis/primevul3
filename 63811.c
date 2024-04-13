static void t1_getline(void)
{
    int c, l, eexec_scan;
    char *p;
    static const char eexec_str[] = "currentfile eexec";
    static int eexec_len = 17;
  restart:
    if (t1_eof())
        normal_error("type 1","unexpected end of file");
    t1_line_ptr = t1_line_array;
    alloc_array(t1_line, 1, T1_BUF_SIZE);
    t1_cslen = 0;
    eexec_scan = 0;
    c = t1_getbyte();
    if (c == EOF)
        goto exit;
    while (!t1_eof()) {
        if (t1_in_eexec == 1)
            c = edecrypt((byte) c);
        alloc_array(t1_line, 1, T1_BUF_SIZE);
        {
            char cc = (char) c;
            append_char_to_buf(cc, t1_line_ptr, t1_line_array, t1_line_limit);
        }
        if (t1_in_eexec == 0 && eexec_scan >= 0 && eexec_scan < eexec_len) {
            if (t1_line_array[eexec_scan] == eexec_str[eexec_scan])
                eexec_scan++;
            else
                eexec_scan = -1;
        }
        if (c == 10 || c == 13
            || (t1_pfa && eexec_scan == eexec_len && c == 32)) {
            break;
        }
        if (t1_cs && t1_cslen == 0 && (t1_line_ptr - t1_line_array > 4) &&
            (t1_suffix(" RD ") || t1_suffix(" -| "))) {
            p = t1_line_ptr - 5;
            while (*p != ' ')
                p--;
            l = (int) t1_scan_num(p + 1, 0);
            t1_cslen = (unsigned short) l;
            /*tex |cs_start| is an index now */
            cs_start = (int) (t1_line_ptr - t1_line_array);
            alloc_array(t1_line, l, T1_BUF_SIZE);
            while (l-- > 0)
                *t1_line_ptr++ = (t1_line_entry) edecrypt((byte) t1_getbyte());
        }
        c = t1_getbyte();
    }
    /*tex |append_eol| can append 2 chars */
    alloc_array(t1_line, 2, T1_BUF_SIZE);
    append_eol(t1_line_ptr, t1_line_array, t1_line_limit);
    if (t1_line_ptr - t1_line_array < 2)
        goto restart;
    if (eexec_scan == eexec_len)
        t1_in_eexec = 1;
  exit:
    /*tex Ensure that |t1_buf_array| has as much room as |t1_line_array|. */
    t1_buf_ptr = t1_buf_array;
    alloc_array(t1_buf, t1_line_limit, t1_line_limit);
}
