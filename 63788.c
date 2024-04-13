static void t1_subset_charstrings(void)
{
    cs_entry *ptr;

    /* at this point t1_line_array contains "/CharStrings".
       when we hit a case like this:
         dup/CharStrings
         229 dict dup begin
       we read the next line and concatenate to t1_line_array before moving on
    */
    t1_check_unusual_charstring();

    cs_size_pos = strstr(t1_line_array, charstringname)
                  + strlen(charstringname) - t1_line_array + 1;
    /* cs_size_pos points to the number indicating
       dict size after "/CharStrings" */
    cs_size = t1_scan_num(t1_line_array + cs_size_pos, 0);
    cs_ptr = cs_tab = xtalloc(cs_size, cs_entry);
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
            pdftex_fail
                ("This Type 1 font uses mismatched subroutine begin/end token pairs.");
        t1_subr_flush();
    }
    for (cs_count = 0, ptr = cs_tab; ptr < cs_ptr; ptr++)
        if (ptr->used)
            cs_count++;
    t1_cs_flush();
}
