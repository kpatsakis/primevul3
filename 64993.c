cmsBool CMSEXPORT cmsIT8SetSheetType(cmsHANDLE hIT8, const char* Type)
{
        TABLE* t = GetTable((cmsIT8*) hIT8);

        strncpy(t ->SheetType, Type, MAXSTR-1);
        t ->SheetType[MAXSTR-1] = 0;
        return TRUE;
}
