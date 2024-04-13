cmsBool DataFormatSection(cmsIT8* it8)
{
    int iField = 0;
    TABLE* t = GetTable(it8);

    InSymbol(it8);   // Eats "BEGIN_DATA_FORMAT"
    CheckEOLN(it8);

    while (it8->sy != SEND_DATA_FORMAT &&
        it8->sy != SEOLN &&
        it8->sy != SEOF &&
        it8->sy != SSYNERROR)  {

            if (it8->sy != SIDENT) {

                return SynError(it8, "Sample type expected");
            }

            if (!SetDataFormat(it8, iField, it8->id)) return FALSE;
            iField++;

            InSymbol(it8);
            SkipEOLN(it8);
       }

       SkipEOLN(it8);
       Skip(it8, SEND_DATA_FORMAT);
       SkipEOLN(it8);

       if (iField != t ->nSamples) {
           SynError(it8, "Count mismatch. NUMBER_OF_FIELDS was %d, found %d\n", t ->nSamples, iField);


       }

       return TRUE;
}
