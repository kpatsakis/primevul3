int CMSEXPORT cmsIT8FindDataFormat(cmsHANDLE hIT8, const char* cSample)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;

    _cmsAssert(hIT8 != NULL);

    return LocateSample(it8, cSample);
}
