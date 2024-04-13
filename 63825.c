static void t1_subset_end(PDF pdf)
{
    if (t1_synthetic) {
        /*tex Copy to |dup /FontName get exch definefont pop|. */
        while (!strstr(t1_line_array, "definefont")) {
            t1_getline();
            t1_putline(pdf);
        }
        while (!t1_end_eexec()) {
            /*tex Ignore the rest. */
            t1_getline();
        }
        /*tex Write \.{mark currentfile closefile}. */
        t1_putline(pdf);
    } else {
        while (!t1_end_eexec()) {
            /*tex Copy to \.{mark currentfile closefile}. */
            t1_getline();
            t1_putline(pdf);
        }
    }
    t1_stop_eexec(pdf);
    if (fixedcontent) {
        /*tex Copy 512 zeros (not needed for PDF). */
        while (!t1_cleartomark()) {
            t1_getline();
            t1_putline(pdf);
        }
        /*tex Don't check \.{{restore}if} for synthetic fonts. */
        if (!t1_synthetic) {
            /*tex Write \.{{restore}if} if found. */
            t1_check_end(pdf);
        }
    }
    get_length3();
}
