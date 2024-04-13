void Skip(cmsIT8* it8, SYMBOL sy)
{
        if (it8->sy == sy && it8->sy != SEOF)
                        InSymbol(it8);
}
