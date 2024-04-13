static void cc_init(void)
{
    int i;
    if (is_cc_init)
        return;
    for (i = 0; i < CS_MAX; i++)
        cc_tab[i].valid = false;
    set_cc(CS_HSTEM, true, 2, true);
    set_cc(CS_VSTEM, true, 2, true);
    set_cc(CS_VMOVETO, true, 1, true);
    set_cc(CS_RLINETO, true, 2, true);
    set_cc(CS_HLINETO, true, 1, true);
    set_cc(CS_VLINETO, true, 1, true);
    set_cc(CS_RRCURVETO, true, 6, true);
    set_cc(CS_CLOSEPATH, false, 0, true);
    set_cc(CS_CALLSUBR, false, 1, false);
    set_cc(CS_RETURN, false, 0, false);
    set_cc(CS_HSBW, true, 2, true);
    set_cc(CS_ENDCHAR, false, 0, true);
    set_cc(CS_RMOVETO, true, 2, true);
    set_cc(CS_HMOVETO, true, 1, true);
    set_cc(CS_VHCURVETO, true, 4, true);
    set_cc(CS_HVCURVETO, true, 4, true);
    set_cc(CS_DOTSECTION, false, 0, true);
    set_cc(CS_VSTEM3, true, 6, true);
    set_cc(CS_HSTEM3, true, 6, true);
    set_cc(CS_SEAC, true, 5, true);
    set_cc(CS_SBW, true, 4, true);
    set_cc(CS_DIV, false, 2, false);
    set_cc(CS_CALLOTHERSUBR, false, 0, false);
    set_cc(CS_POP, false, 0, false);
    set_cc(CS_SETCURRENTPOINT, true, 2, true);
    is_cc_init = true;
}
