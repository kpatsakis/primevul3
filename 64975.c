cmsFloat64Number CMSEXPORT cmsIT8GetDataRowColDbl(cmsHANDLE hIT8, int row, int col)
{
    const char* Buffer;

    Buffer = cmsIT8GetDataRowCol(hIT8, row, col);

    if (Buffer == NULL) return 0.0;

    return ParseFloatNumber(Buffer);
}
