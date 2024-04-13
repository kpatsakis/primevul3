cmsUInt32Number CMSEXPORT cmsIT8EnumPropertyMulti(cmsHANDLE hIT8, const char* cProp, const char ***SubpropertyNames)
{
    cmsIT8* it8 = (cmsIT8*) hIT8;
    KEYVALUE *p, *tmp;
    cmsUInt32Number n;
    const char **Props;
    TABLE* t;

    _cmsAssert(hIT8 != NULL);


    t = GetTable(it8);

    if(!IsAvailableOnList(t->HeaderList, cProp, NULL, &p)) {
        *SubpropertyNames = 0;
        return 0;
    }


    n = 0;
    for (tmp = p;  tmp != NULL; tmp = tmp->NextSubkey) {
        if(tmp->Subkey != NULL)
            n++;
    }


    Props = (const char **) AllocChunk(it8, sizeof(char *) * n);

    n = 0;
    for (tmp = p;  tmp != NULL; tmp = tmp->NextSubkey) {
        if(tmp->Subkey != NULL)
            Props[n++] = p ->Subkey;
    }

    *SubpropertyNames = Props;
    return n;
}
