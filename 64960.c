cmsBool SetDataFormat(cmsIT8* it8, int n, const char *label)
{
    TABLE* t = GetTable(it8);

    if (!t->DataFormat)
        AllocateDataFormat(it8);

    if (n > t -> nSamples) {
        SynError(it8, "More than NUMBER_OF_FIELDS fields.");
        return FALSE;
    }

    if (t->DataFormat) {
        t->DataFormat[n] = AllocString(it8, label);
    }

    return TRUE;
}
