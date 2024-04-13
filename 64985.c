cmsBool CMSEXPORT cmsIT8SetData(cmsHANDLE hIT8, const char* cPatch, const char* cSample, const char *Val)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;
    int iField, iSet;
    TABLE* t;

    _cmsAssert(hIT8 != NULL);

    t = GetTable(it8);

    iField = LocateSample(it8, cSample);

    if (iField < 0)
        return FALSE;

    if (t-> nPatches == 0) {

        AllocateDataFormat(it8);
        AllocateDataSet(it8);
        CookPointers(it8);
    }

    if (cmsstrcasecmp(cSample, "SAMPLE_ID") == 0) {

        iSet   = LocateEmptyPatch(it8);
        if (iSet < 0) {
            return SynError(it8, "Couldn't add more patches '%s'\n", cPatch);
        }

        iField = t -> SampleID;
    }
    else {
        iSet = LocatePatch(it8, cPatch);
        if (iSet < 0) {
            return FALSE;
        }
    }

    return SetData(it8, iSet, iField, Val);
}
