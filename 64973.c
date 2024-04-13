void CMSEXPORT cmsIT8Free(cmsHANDLE hIT8)
{
   cmsIT8* it8 = (cmsIT8*) hIT8;

    if (it8 == NULL)
        return;

    if (it8->MemorySink) {

        OWNEDMEM* p;
        OWNEDMEM* n;

        for (p = it8->MemorySink; p != NULL; p = n) {

            n = p->Next;
            if (p->Ptr) _cmsFree(it8 ->ContextID, p->Ptr);
            _cmsFree(it8 ->ContextID, p);
        }
    }

    if (it8->MemoryBlock)
        _cmsFree(it8 ->ContextID, it8->MemoryBlock);

    _cmsFree(it8 ->ContextID, it8);
}
