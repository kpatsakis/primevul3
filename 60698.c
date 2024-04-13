cmsBool Type_Dictionary_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
{
    cmsHANDLE hDict = (cmsHANDLE) Ptr;
    const cmsDICTentry* p;
    cmsBool AnyName, AnyValue;
    cmsUInt32Number i, Count, Length;
    cmsUInt32Number DirectoryPos, CurrentPos, BaseOffset;
   _cmsDICarray a;

    if (hDict == NULL) return FALSE;

    BaseOffset = io ->Tell(io) - sizeof(_cmsTagBase);

    Count = 0; AnyName = FALSE; AnyValue = FALSE;
    for (p = cmsDictGetEntryList(hDict); p != NULL; p = cmsDictNextEntry(p)) {

        if (p ->DisplayName != NULL) AnyName = TRUE;
        if (p ->DisplayValue != NULL) AnyValue = TRUE;
        Count++;
    }

    Length = 16;
    if (AnyName)  Length += 8;
    if (AnyValue) Length += 8;

    if (!_cmsWriteUInt32Number(io, Count)) return FALSE;
    if (!_cmsWriteUInt32Number(io, Length)) return FALSE;

    DirectoryPos = io ->Tell(io);

    if (!AllocArray(self ->ContextID, &a, Count, Length)) goto Error;

    if (!WriteOffsetArray(io, &a, Count, Length)) goto Error;

    p = cmsDictGetEntryList(hDict);
    for (i=0; i < Count; i++) {

        if (!WriteOneWChar(io, &a.Name, i,  p ->Name, BaseOffset)) goto Error;
        if (!WriteOneWChar(io, &a.Value, i, p ->Value, BaseOffset)) goto Error;

        if (p ->DisplayName != NULL) {
            if (!WriteOneMLUC(self, io, &a.DisplayName, i, p ->DisplayName, BaseOffset)) goto Error;
        }

        if (p ->DisplayValue != NULL) {
            if (!WriteOneMLUC(self, io, &a.DisplayValue, i, p ->DisplayValue, BaseOffset)) goto Error;
        }

       p = cmsDictNextEntry(p);
    }

    CurrentPos = io ->Tell(io);
    if (!io ->Seek(io, DirectoryPos)) goto Error;

    if (!WriteOffsetArray(io, &a, Count, Length)) goto Error;

    if (!io ->Seek(io, CurrentPos)) goto Error;

    FreeArray(&a);
    return TRUE;

Error:
    FreeArray(&a);
    return FALSE;

    cmsUNUSED_PARAMETER(nItems);
}
