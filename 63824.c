static void t1_subset_charstrings(PDF pdf)
{
    cs_entry *ptr;
    t1_check_unusual_charstring();
    cs_size_pos = (int) (strstr(t1_line_array, charstringname) + strlen(charstringname) - t1_line_array + 1);
    /*tex |cs_size_pos| points to the number indicating dict size after |/CharStrings|. */
    cs_size = (int) t1_scan_num(t1_line_array + cs_size_pos, 0);
    cs_ptr = cs_tab = xtalloc((unsigned) cs_size, cs_entry);
    for (ptr = cs_tab; ptr - cs_tab < cs_size; ptr++)
        init_cs_entry(ptr);
    cs_notdef = NULL;
    cs_dict_start = xstrdup(t1_line_array);
    t1_getline();
    while (t1_cslen) {
        store_cs();
        t1_getline();
    }
    cs_dict_end = xstrdup(t1_line_array);
    t1_mark_glyphs();
    if (subr_tab != NULL) {
        if (cs_token_pair == NULL)
            formatted_error("type 1","mismatched subroutine begin/end token pairs");
        t1_subr_flush();
    }
    for (cs_counter = 0, ptr = cs_tab; ptr < cs_ptr; ptr++)
        if (ptr->used)
            cs_counter++;
    t1_cs_flush();
}
