void* AllocBigBlock(cmsIT8* it8, cmsUInt32Number size)
{
    OWNEDMEM* ptr1;
    void* ptr = _cmsMallocZero(it8->ContextID, size);

    if (ptr != NULL) {

        ptr1 = (OWNEDMEM*) _cmsMallocZero(it8 ->ContextID, sizeof(OWNEDMEM));

        if (ptr1 == NULL) {

            _cmsFree(it8 ->ContextID, ptr);
            return NULL;
        }

        ptr1-> Ptr        = ptr;
        ptr1-> Next       = it8 -> MemorySink;
        it8 -> MemorySink = ptr1;
    }

    return ptr;
}
