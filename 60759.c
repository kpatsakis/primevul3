cmsBool Type_Text_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
{
    cmsMLU* mlu = (cmsMLU*) Ptr;
    cmsUInt32Number size;
    cmsBool  rc;
    char* Text;

    size = cmsMLUgetASCII(mlu, cmsNoLanguage, cmsNoCountry, NULL, 0);
    if (size == 0) return FALSE;       // Cannot be zero!

    Text = (char*) _cmsMalloc(self ->ContextID, size);
    if (Text == NULL) return FALSE;

    cmsMLUgetASCII(mlu, cmsNoLanguage, cmsNoCountry, Text, size);

    rc = io ->Write(io, size, Text);

    _cmsFree(self ->ContextID, Text);
    return rc;

    cmsUNUSED_PARAMETER(nItems);
}
