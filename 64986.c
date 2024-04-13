cmsBool CMSEXPORT cmsIT8SetDataFormat(cmsHANDLE  h, int n, const char *Sample)
{
    cmsIT8* it8 = (cmsIT8*)h;
    return SetDataFormat(it8, n, Sample);
}
