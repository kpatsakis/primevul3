cmsHANDLE  CMSEXPORT cmsIT8LoadFromMem(cmsContext ContextID, const void *Ptr, cmsUInt32Number len)
{
    cmsHANDLE hIT8;
    cmsIT8*  it8;
    int type;

    _cmsAssert(Ptr != NULL);
    _cmsAssert(len != 0);

    type = IsMyBlock((const cmsUInt8Number*)Ptr, len);
    if (type == 0) return NULL;

    hIT8 = cmsIT8Alloc(ContextID);
    if (!hIT8) return NULL;

    it8 = (cmsIT8*) hIT8;
    it8 ->MemoryBlock = (char*) _cmsMalloc(ContextID, len + 1);
    if (it8->MemoryBlock == NULL)
    {
        cmsIT8Free(hIT8);
        return FALSE;
    }

    strncpy(it8 ->MemoryBlock, (const char*) Ptr, len);
    it8 ->MemoryBlock[len] = 0;

    strncpy(it8->FileStack[0]->FileName, "", cmsMAX_PATH-1);
    it8-> Source = it8 -> MemoryBlock;

    if (!ParseIT8(it8, type-1)) {

        cmsIT8Free(hIT8);
        return FALSE;
    }

    CookPointers(it8);
    it8 ->nTable = 0;

    _cmsFree(ContextID, it8->MemoryBlock);
    it8 -> MemoryBlock = NULL;

    return hIT8;


}
