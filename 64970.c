cmsUInt32Number CMSEXPORT cmsIT8EnumProperties(cmsHANDLE hIT8, char ***PropertyNames)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;
    KEYVALUE* p;
    cmsUInt32Number n;
    char **Props;
    TABLE* t;

    _cmsAssert(hIT8 != NULL);

    t = GetTable(it8);


    n = 0;
    for (p = t -> HeaderList;  p != NULL; p = p->Next) {
        n++;
    }


    Props = (char **) AllocChunk(it8, sizeof(char *) * n);

    n = 0;
    for (p = t -> HeaderList;  p != NULL; p = p->Next) {
        Props[n++] = p -> Keyword;
    }

    *PropertyNames = Props;
    return n;
}
