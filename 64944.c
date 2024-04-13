cmsBool DataSection (cmsIT8* it8)
{
    int  iField = 0;
    int  iSet   = 0;
    char Buffer[256];
    TABLE* t = GetTable(it8);

    InSymbol(it8);   // Eats "BEGIN_DATA"
    CheckEOLN(it8);

    if (!t->Data)
        AllocateDataSet(it8);

    while (it8->sy != SEND_DATA && it8->sy != SEOF)
    {
        if (iField >= t -> nSamples) {
            iField = 0;
            iSet++;

        }

        if (it8->sy != SEND_DATA && it8->sy != SEOF) {

            if (!GetVal(it8, Buffer, 255, "Sample data expected"))
                return FALSE;

            if (!SetData(it8, iSet, iField, Buffer))
                return FALSE;

            iField++;

            InSymbol(it8);
            SkipEOLN(it8);
        }
    }

    SkipEOLN(it8);
    Skip(it8, SEND_DATA);
    SkipEOLN(it8);


    if ((iSet+1) != t -> nPatches)
        return SynError(it8, "Count mismatch. NUMBER_OF_SETS was %d, found %d\n", t ->nPatches, iSet+1);

    return TRUE;
}
