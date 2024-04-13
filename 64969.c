int CMSEXPORT cmsIT8EnumDataFormat(cmsHANDLE hIT8, char ***SampleNames)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;
    TABLE* t;

    _cmsAssert(hIT8 != NULL);

    t = GetTable(it8);

    if (SampleNames)
        *SampleNames = t -> DataFormat;
    return t -> nSamples;
}
