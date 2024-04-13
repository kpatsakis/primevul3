const char* CMSEXPORT cmsIT8GetDataRowCol(cmsHANDLE hIT8, int row, int col)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;

    _cmsAssert(hIT8 != NULL);

    return GetData(it8, row, col);
}
