static void t1_read_subrs(void)
{
    int i, s;
    cs_entry *ptr;
    t1_getline();
    while (!(t1_charstrings() || t1_subrs())) {
        t1_scan_param();
        t1_putline();
        t1_getline();
    }
  found:
    t1_cs = true;
    t1_scan = false;
    if (!t1_subrs())
        return;
    subr_size_pos = strlen("/Subrs") + 1;
    /* subr_size_pos points to the number indicating dict size after "/Subrs" */
    subr_size = t1_scan_num(t1_line_array + subr_size_pos, 0);
    if (subr_size == 0) {
        while (!t1_charstrings())
            t1_getline();
        return;
    }
    subr_tab = xtalloc(subr_size, cs_entry);
    for (ptr = subr_tab; ptr - subr_tab < subr_size; ptr++)
        init_cs_entry(ptr);
    subr_array_start = xstrdup(t1_line_array);
    t1_getline();
    while (t1_cslen) {
        store_subr();
        t1_getline();
    }
    /* mark the first four entries without parsing */
    for (i = 0; i < subr_size && i < 4; i++)
        subr_tab[i].used = true;
    /* the end of the Subrs array might have more than one line so we need to
       concatnate them to subr_array_end. Unfortunately some fonts don't have
       the Subrs array followed by the CharStrings dict immediately (synthetic
       fonts). If we cannot find CharStrings in next POST_SUBRS_SCAN lines then
       we will treat the font as synthetic and ignore everything until next
       Subrs is found
     */

#define POST_SUBRS_SCAN  5

    s = 0;
    *t1_buf_array = 0;
    for (i = 0; i < POST_SUBRS_SCAN; i++) {
        if (t1_charstrings())
            break;
        s += t1_line_ptr - t1_line_array;
        alloc_array(t1_buf, s, T1_BUF_SIZE);
        strcat(t1_buf_array, t1_line_array);
        t1_getline();
    }
    subr_array_end = xstrdup(t1_buf_array);
    if (i == POST_SUBRS_SCAN) { /* CharStrings not found;
                                   suppose synthetic font */
        for (ptr = subr_tab; ptr - subr_tab < subr_size; ptr++)
            if (ptr->valid)
                xfree(ptr->data);
        xfree(subr_tab);
        xfree(subr_array_start);
        xfree(subr_array_end);
        cs_init();
        t1_cs = false;
        t1_synthetic = true;
        while (!(t1_charstrings() || t1_subrs()))
            t1_getline();
        goto found;
    }
}
