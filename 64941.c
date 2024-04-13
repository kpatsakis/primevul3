cmsBool CheckEOLN(cmsIT8* it8)
{
        if (!Check(it8, SEOLN, "Expected separator")) return FALSE;
        while (it8 -> sy == SEOLN)
                        InSymbol(it8);
        return TRUE;

}
