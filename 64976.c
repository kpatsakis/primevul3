int CMSEXPORT cmsIT8GetPatchByName(cmsHANDLE hIT8, const char *cPatch)
{
    _cmsAssert(hIT8 != NULL);

    return LocatePatch((cmsIT8*)hIT8, cPatch);
}
