static void t1_include(PDF pdf)
{
    do {
        t1_getline();
        t1_scan_param(pdf);
        t1_putline(pdf);
    }
    while (t1_in_eexec == 0);
    t1_start_eexec(pdf);
    do {
        t1_getline();
        t1_scan_param(pdf);
        t1_putline(pdf);
    }
    while (!(t1_charstrings() || t1_subrs()));
    t1_cs = true;
    do {
        t1_getline();
        t1_putline(pdf);
    }
    while (!t1_end_eexec());
    t1_stop_eexec(pdf);
    if (fixedcontent) {
        /*tex Copy 512 zeros (not needed for \PDF). */
        do {
            t1_getline();
            t1_putline(pdf);
        }
        while (!t1_cleartomark());
        /*tex Write |{restore} if| if found. */
        t1_check_end(pdf);
    }
    get_length3();
}
