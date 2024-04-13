cmsBool  Type_UcrBg_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
{
    cmsUcrBg* Value = (cmsUcrBg*) Ptr;
    cmsUInt32Number TextSize;
    char* Text;

    if (!_cmsWriteUInt32Number(io, Value ->Ucr ->nEntries)) return FALSE;
    if (!_cmsWriteUInt16Array(io, Value ->Ucr ->nEntries, Value ->Ucr ->Table16)) return FALSE;

    if (!_cmsWriteUInt32Number(io, Value ->Bg ->nEntries)) return FALSE;
    if (!_cmsWriteUInt16Array(io, Value ->Bg ->nEntries, Value ->Bg ->Table16)) return FALSE;

    TextSize = cmsMLUgetASCII(Value ->Desc, cmsNoLanguage, cmsNoCountry, NULL, 0);
    Text     = (char*) _cmsMalloc(self ->ContextID, TextSize);
    if (cmsMLUgetASCII(Value ->Desc, cmsNoLanguage, cmsNoCountry, Text, TextSize) != TextSize) return FALSE;

    if (!io ->Write(io, TextSize, Text)) return FALSE;
    _cmsFree(self ->ContextID, Text);

    return TRUE;

    cmsUNUSED_PARAMETER(nItems);
}
