static void cs_store(boolean is_subr)
{
    char *p;
    cs_entry *ptr;
    int subr;
    for (p = t1_line_array, t1_buf_ptr = t1_buf_array; *p != ' ';
         *t1_buf_ptr++ = *p++);
    *t1_buf_ptr = 0;
    if (is_subr) {
        subr = (int) t1_scan_num(p + 1, 0);
        check_subr(subr);
        ptr = subr_tab + subr;
    } else {
        ptr = cs_ptr++;
        if (cs_ptr - cs_tab > cs_size)
            formatted_error("type 1","CharStrings dict: more entries than dict size '%i'", cs_size);
        if (strcmp(t1_buf_array + 1, notdef) == 0)      /* skip the slash */
            ptr->name = (char *) notdef;
        else
            ptr->name = xstrdup(t1_buf_array + 1);
    }
    /*tex Copy |" RD " + cs data| to |t1_buf_array|. */
    memcpy(t1_buf_array, t1_line_array + cs_start - 4, (unsigned) (t1_cslen + 4));
    /*tex Copy the end of cs data to |t1_buf_array|. */
    for (p = t1_line_array + cs_start + t1_cslen, t1_buf_ptr =
         t1_buf_array + t1_cslen + 4; *p != 10; *t1_buf_ptr++ = *p++);
    *t1_buf_ptr++ = 10;
    if (is_subr && cs_token_pair == NULL)
        cs_token_pair = check_cs_token_pair();
    ptr->len = (unsigned short) (t1_buf_ptr - t1_buf_array);
    ptr->cslen = t1_cslen;
    xfree(ptr->data);
    ptr->data = xtalloc(ptr->len, byte);
    memcpy(ptr->data, t1_buf_array, ptr->len);
    ptr->valid = true;
}
