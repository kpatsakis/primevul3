cmsBool CMSEXPORT cmsIT8SetPropertyDbl(cmsHANDLE hIT8, const char* cProp, cmsFloat64Number Val)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;
    char Buffer[1024];

    snprintf(Buffer, 1023, it8->DoubleFormatter, Val);

    return AddToList(it8, &GetTable(it8)->HeaderList, cProp, NULL, Buffer, WRITE_UNCOOKED) != NULL;
}
