static void t1_subset_end(void)
{
    if (t1_synthetic) {         /* copy to "dup /FontName get exch definefont pop" */
        while (!strstr(t1_line_array, "definefont")) {
            t1_getline();
            t1_putline();
        }
        while (!t1_end_eexec())
            t1_getline();       /* ignore the rest */
        t1_putline();           /* write "mark currentfile closefile" */
    } else
        while (!t1_end_eexec()) {       /* copy to "mark currentfile closefile" */
            t1_getline();
            t1_putline();
        }
    t1_stop_eexec();
    if (fixedcontent) {         /* copy 512 zeros (not needed for PDF) */
        while (!t1_cleartomark()) {
            t1_getline();
            t1_putline();
        }
        if (!t1_synthetic)      /* don't check "{restore}if" for synthetic fonts */
            t1_check_end();     /* write "{restore}if" if found */
    }
    get_length3();
}
