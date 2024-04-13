cmsBool CMSEXPORT cmsIT8SetPropertyUncooked(cmsHANDLE hIT8, const char* Key, const char* Buffer)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;

    return AddToList(it8, &GetTable(it8)->HeaderList, Key, NULL, Buffer, WRITE_UNCOOKED) != NULL;
}
