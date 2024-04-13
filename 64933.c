KEYVALUE* AddToList(cmsIT8* it8, KEYVALUE** Head, const char *Key, const char *Subkey, const char* xValue, WRITEMODE WriteAs)
{
    KEYVALUE* p;
    KEYVALUE* last;



    if (IsAvailableOnList(*Head, Key, Subkey, &p)) {


    }
    else {

        last = p;

        p = (KEYVALUE*) AllocChunk(it8, sizeof(KEYVALUE));
        if (p == NULL)
        {
            SynError(it8, "AddToList: out of memory");
            return NULL;
        }

        p->Keyword = AllocString(it8, Key);
        p->Subkey = (Subkey == NULL) ? NULL : AllocString(it8, Subkey);

        if (*Head == NULL) {
            *Head = p;
        }
        else
        {
            if (Subkey != NULL && last != NULL) {

                last->NextSubkey = p;

                while (last->Next != NULL)
                         last = last->Next;
            }

            if (last != NULL) last->Next = p;
        }

        p->Next    = NULL;
        p->NextSubkey = NULL;
    }

    p->WriteAs = WriteAs;

    if (xValue != NULL) {

        p->Value   = AllocString(it8, xValue);
    }
    else {
        p->Value   = NULL;
    }

    return p;
}
