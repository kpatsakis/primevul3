cmsBool CMSEXPORT cmsIT8SetPropertyMulti(cmsHANDLE hIT8, const char* Key, const char* SubKey, const char *Buffer)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;

    return AddToList(it8, &GetTable(it8)->HeaderList, Key, SubKey, Buffer, WRITE_PAIR) != NULL;
}
