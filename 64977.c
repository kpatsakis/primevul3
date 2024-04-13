const char* CMSEXPORT cmsIT8GetPatchName(cmsHANDLE hIT8, int nPatch, char* buffer)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;
    TABLE* t;
    char* Data;

    _cmsAssert(hIT8 != NULL);

    t = GetTable(it8);
    Data = GetData(it8, nPatch, t->SampleID);

    if (!Data) return NULL;
    if (!buffer) return Data;

    strncpy(buffer, Data, MAXSTR-1);
    buffer[MAXSTR-1] = 0;
    return buffer;
}
