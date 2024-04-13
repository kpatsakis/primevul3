cmsUInt32Number CMSEXPORT cmsIT8TableCount(cmsHANDLE hIT8)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;

    _cmsAssert(hIT8 != NULL);

    return it8 ->TablesCount;
}
