cmsBool CMSEXPORT cmsIT8SetDataRowColDbl(cmsHANDLE hIT8, int row, int col, cmsFloat64Number Val)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;
    char Buff[256];

    _cmsAssert(hIT8 != NULL);

    snprintf(Buff, 255, it8->DoubleFormatter, Val);

    return SetData(it8, row, col, Buff);
}
