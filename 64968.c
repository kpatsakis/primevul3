void CMSEXPORT cmsIT8DefineDblFormat(cmsHANDLE hIT8, const char* Formatter)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;

    _cmsAssert(hIT8 != NULL);

    if (Formatter == NULL)
        strcpy(it8->DoubleFormatter, DEFAULT_DBL_FORMAT);
    else
        strncpy(it8->DoubleFormatter, Formatter, sizeof(it8->DoubleFormatter));

    it8 ->DoubleFormatter[sizeof(it8 ->DoubleFormatter)-1] = 0;
}
