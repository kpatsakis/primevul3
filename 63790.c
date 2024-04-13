static void writet1(void)
{
    read_encoding_only = false;
    if (!is_included(fm_cur)) { /* scan parameters from font file */
        if (!t1_open_fontfile("{"))
            return;
        t1_scan_only();
        t1_close_font_file("}");
        return;
    }
    if (!is_subsetted(fm_cur)) { /* include entire font */
        if (!t1_open_fontfile("<<"))
            return;
        t1_include();
        t1_close_font_file(">>");
        return;
    }
    /* partial downloading */
    if (!t1_open_fontfile("<"))
        return;
    t1_subset_ascii_part();
    t1_start_eexec();
    cc_init();
    cs_init();
    t1_read_subrs();
    t1_subset_charstrings();
    t1_subset_end();
    t1_close_font_file(">");
}
