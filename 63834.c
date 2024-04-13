static void t1_include(void)
{
    do {
        t1_getline();
        t1_scan_param();
        t1_putline();
    } while (t1_in_eexec == 0);
    t1_start_eexec();
    do {
        t1_getline();
        t1_scan_param();
        t1_putline();
    } while (!(t1_charstrings() || t1_subrs()));
    t1_cs = true;
    do {
        t1_getline();
        t1_putline();
    } while (!t1_end_eexec());
    t1_stop_eexec();
    if (fixedcontent) {         /* copy 512 zeros (not needed for PDF) */
        do {
            t1_getline();
            t1_putline();
        } while (!t1_cleartomark());
        t1_check_end();         /* write "{restore}if" if found */
    }
    get_length3();
}
