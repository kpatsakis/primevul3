cmsBool CMSEXPORT cmsIT8SetPropertyStr(cmsHANDLE hIT8, const char* Key, const char *Val)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;

    if (!Val) return FALSE;
    if (!*Val) return FALSE;

    return AddToList(it8, &GetTable(it8)->HeaderList, Key, NULL, Val, WRITE_STRINGIFY) != NULL;
}
