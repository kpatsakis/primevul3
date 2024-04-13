cmsBool CMSEXPORT cmsIT8SetComment(cmsHANDLE hIT8, const char* Val)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;

    if (!Val) return FALSE;
    if (!*Val) return FALSE;

    return AddToList(it8, &GetTable(it8)->HeaderList, "# ", NULL, Val, WRITE_UNCOOKED) != NULL;
}
