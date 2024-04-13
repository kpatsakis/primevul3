void *Type_Text_Description_Read(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number* nItems, cmsUInt32Number SizeOfTag)
{
    char* Text = NULL;
    cmsMLU* mlu = NULL;
    cmsUInt32Number  AsciiCount;
    cmsUInt32Number  i, UnicodeCode, UnicodeCount;
    cmsUInt16Number  ScriptCodeCode, Dummy;
    cmsUInt8Number   ScriptCodeCount;

    *nItems = 0;

    if (SizeOfTag < sizeof(cmsUInt32Number)) return NULL;

    if (!_cmsReadUInt32Number(io, &AsciiCount)) return NULL;
    SizeOfTag -= sizeof(cmsUInt32Number);

    if (SizeOfTag < AsciiCount) return NULL;

    mlu = cmsMLUalloc(self ->ContextID, 1);
    if (mlu == NULL) return NULL;

    Text = (char*) _cmsMalloc(self ->ContextID, AsciiCount + 1);
    if (Text == NULL) goto Error;

    if (io ->Read(io, Text, sizeof(char), AsciiCount) != AsciiCount) goto Error;
    SizeOfTag -= AsciiCount;

    Text[AsciiCount] = 0;

    if (!cmsMLUsetASCII(mlu, cmsNoLanguage, cmsNoCountry, Text)) goto Error;
    _cmsFree(self ->ContextID, (void*) Text);
    Text = NULL;

    if (SizeOfTag < 2* sizeof(cmsUInt32Number)) goto Done;
    if (!_cmsReadUInt32Number(io, &UnicodeCode)) goto Done;
    if (!_cmsReadUInt32Number(io, &UnicodeCount)) goto Done;
    SizeOfTag -= 2* sizeof(cmsUInt32Number);

    if (SizeOfTag < UnicodeCount*sizeof(cmsUInt16Number)) goto Done;

    for (i=0; i < UnicodeCount; i++) {
        if (!io ->Read(io, &Dummy, sizeof(cmsUInt16Number), 1)) goto Done;
    }
    SizeOfTag -= UnicodeCount*sizeof(cmsUInt16Number);


    if (SizeOfTag >= sizeof(cmsUInt16Number) + sizeof(cmsUInt8Number) + 67) {

        if (!_cmsReadUInt16Number(io, &ScriptCodeCode)) goto Done;
        if (!_cmsReadUInt8Number(io,  &ScriptCodeCount)) goto Done;

        for (i=0; i < 67; i++) {
            if (!io ->Read(io, &Dummy, sizeof(cmsUInt8Number), 1)) goto Error;
        }
    }

Done:

    *nItems = 1;
    return mlu;

Error:
    if (Text) _cmsFree(self ->ContextID, (void*) Text);
    if (mlu) cmsMLUfree(mlu);
    return NULL;
}
