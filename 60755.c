cmsBool  Type_Text_Description_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
{
    cmsMLU* mlu = (cmsMLU*) Ptr;
    char *Text = NULL;
    wchar_t *Wide = NULL;
    cmsUInt32Number len, len_text, len_tag_requirement, len_aligned;
    cmsBool  rc = FALSE;
    char Filler[68];

    memset(Filler, 0, sizeof(Filler));

    len = cmsMLUgetASCII(mlu, cmsNoLanguage, cmsNoCountry, NULL, 0);


    if (len <= 0) {

        Text = (char*)    _cmsDupMem(self ->ContextID, "", sizeof(char));
        Wide = (wchar_t*) _cmsDupMem(self ->ContextID, L"", sizeof(wchar_t));
    }
    else {
        Text = (char*) _cmsCalloc(self ->ContextID, len, sizeof(char));
        if (Text == NULL) goto Error;

        Wide = (wchar_t*) _cmsCalloc(self ->ContextID, len, sizeof(wchar_t));
        if (Wide == NULL) goto Error;

        cmsMLUgetASCII(mlu, cmsNoLanguage, cmsNoCountry,  Text, len * sizeof(char));
        cmsMLUgetWide(mlu,  cmsNoLanguage, cmsNoCountry,  Wide, len * sizeof(wchar_t));
    }

    len_text = (cmsUInt32Number) strlen(Text) + 1;
    len_tag_requirement = (8+4+len_text+4+4+2*len_text+2+1+67);
    len_aligned = _cmsALIGNLONG(len_tag_requirement);


    if (!_cmsWriteUInt32Number(io, len_text)) goto Error;
    if (!io ->Write(io, len_text, Text)) goto Error;

    if (!_cmsWriteUInt32Number(io, 0)) goto Error;  // ucLanguageCode

    if (!_cmsWriteUInt32Number(io, len_text)) goto Error;
    if (!_cmsWriteWCharArray(io, len_text, Wide)) goto Error;

    if (!_cmsWriteUInt16Number(io, 0)) goto Error;
    if (!_cmsWriteUInt8Number(io, 0)) goto Error;

    if (!io ->Write(io, 67, Filler)) goto Error;

    if(len_aligned - len_tag_requirement > 0)
      if (!io ->Write(io, len_aligned - len_tag_requirement, Filler)) goto Error;

    rc = TRUE;

Error:
    if (Text) _cmsFree(self ->ContextID, Text);
    if (Wide) _cmsFree(self ->ContextID, Wide);

    return rc;

    cmsUNUSED_PARAMETER(nItems);
}
