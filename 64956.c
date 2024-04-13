cmsBool ParseIT8(cmsIT8* it8, cmsBool nosheet)
{
    char* SheetTypePtr = it8 ->Tab[0].SheetType;

    if (nosheet == 0) {
        ReadType(it8, SheetTypePtr);
    }

    InSymbol(it8);

    SkipEOLN(it8);

    while (it8-> sy != SEOF &&
           it8-> sy != SSYNERROR) {

            switch (it8 -> sy) {

            case SBEGIN_DATA_FORMAT:
                    if (!DataFormatSection(it8)) return FALSE;
                    break;

            case SBEGIN_DATA:

                    if (!DataSection(it8)) return FALSE;

                    if (it8 -> sy != SEOF) {

                            AllocTable(it8);
                            it8 ->nTable = it8 ->TablesCount - 1;

                            if (nosheet == 0) {

                                if (it8 ->sy == SIDENT) {

                                     while (isseparator(it8->ch))
                                         NextCh(it8);

                                    if (it8 ->ch == '\n' || it8->ch == '\r') {

                                         cmsIT8SetSheetType(it8, it8 ->id);
                                         InSymbol(it8);
                                    }
                                    else
                                    {
                                        cmsIT8SetSheetType(it8, "");
                                    }
                                }
                                else
                                    if (it8 ->sy == SSTRING) {
                                        cmsIT8SetSheetType(it8, it8 ->str);
                                        InSymbol(it8);
                                    }
                           }

                    }
                    break;

            case SEOLN:
                    SkipEOLN(it8);
                    break;

            default:
                    if (!HeaderSection(it8)) return FALSE;
           }

    }

    return (it8 -> sy != SSYNERROR);
}
