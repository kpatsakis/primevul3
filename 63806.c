static void t1_check_end(PDF pdf)
{
    if (t1_eof())
        return;
    t1_getline();
    if (t1_prefix("{restore}"))
        t1_putline(pdf);
}
