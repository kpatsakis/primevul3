int CMSEXPORT cmsIT8SetTableByLabel(cmsHANDLE hIT8, const char* cSet, const char* cField, const char* ExpectedType)
{
    const char* cLabelFld;
    char Type[256], Label[256];
    cmsUInt32Number nTable;

    _cmsAssert(hIT8 != NULL);

    if (cField != NULL && *cField == 0)
            cField = "LABEL";

    if (cField == NULL)
            cField = "LABEL";

    cLabelFld = cmsIT8GetData(hIT8, cSet, cField);
    if (!cLabelFld) return -1;

    if (sscanf(cLabelFld, "%255s %u %255s", Label, &nTable, Type) != 3)
            return -1;

    if (ExpectedType != NULL && *ExpectedType == 0)
        ExpectedType = NULL;

    if (ExpectedType) {

        if (cmsstrcasecmp(Type, ExpectedType) != 0) return -1;
    }

    return cmsIT8SetTable(hIT8, nTable);
}
