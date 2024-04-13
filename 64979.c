const char* CMSEXPORT cmsIT8GetPropertyMulti(cmsHANDLE hIT8, const char* Key, const char *SubKey)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;
    KEYVALUE* p;

    if (IsAvailableOnList(GetTable(it8) -> HeaderList, Key, SubKey, &p)) {
        return p -> Value;
    }
    return NULL;
}
