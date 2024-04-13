static void t1_check_pfa(void)
{
    const int c = t1_getchar();
    t1_pfa = (c != 128) ? true : false;
    t1_ungetchar(c);
}
