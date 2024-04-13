cmsBool IsAvailableOnList(KEYVALUE* p, const char* Key, const char* Subkey, KEYVALUE** LastPtr)
{
    if (LastPtr) *LastPtr = p;

    for (;  p != NULL; p = p->Next) {

        if (LastPtr) *LastPtr = p;

        if (*Key != '#') { // Comments are ignored

            if (cmsstrcasecmp(Key, p->Keyword) == 0)
                break;
        }
    }

    if (p == NULL)
        return FALSE;

    if (Subkey == 0)
        return TRUE;

    for (; p != NULL; p = p->NextSubkey) {

        if (p ->Subkey == NULL) continue;

        if (LastPtr) *LastPtr = p;

        if (cmsstrcasecmp(Subkey, p->Subkey) == 0)
            return TRUE;
    }

    return FALSE;
}
