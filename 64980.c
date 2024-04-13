const char* CMSEXPORT cmsIT8GetSheetType(cmsHANDLE hIT8)
{
        return GetTable((cmsIT8*) hIT8)->SheetType;
}
