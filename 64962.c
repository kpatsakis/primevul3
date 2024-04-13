void SkipEOLN(cmsIT8* it8)
{
    while (it8->sy == SEOLN) {
             InSymbol(it8);
    }
}
