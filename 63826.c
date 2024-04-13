void writet1(PDF pdf, fd_entry * fd)
{
    /*tex |fd_cur| is global inside |writet1.c|. */
    fd_cur = fd;
    assert(fd_cur->fm != NULL);
    assert(is_type1(fd->fm));
    assert(is_included(fd->fm));

    t1_save_offset = 0;
    if (!is_subsetted(fd_cur->fm)) {
        /*tex Include entire font. */
        if (!(fd->ff_found = t1_open_fontfile(filetype_font)))
            return;
        t1_include(pdf);
        t1_close_font_file(filetype_font);
        xfree(t1_buffer);
        return;
    }
    /*tex Partial downloading. */
    if (!(fd->ff_found = t1_open_fontfile(filetype_subset)))
        return;
    t1_subset_ascii_part(pdf);
    t1_start_eexec(pdf);
    cc_init();
    cs_init();
    t1_read_subrs(pdf);
    t1_subset_charstrings(pdf);
    t1_subset_end(pdf);
    t1_close_font_file(filetype_subset);
    xfree(t1_buffer);
}
