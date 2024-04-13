void *Type_MPE_Read(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number* nItems, cmsUInt32Number SizeOfTag)
{
    cmsUInt16Number InputChans, OutputChans;
    cmsUInt32Number ElementCount;
    cmsPipeline *NewLUT = NULL;
    cmsUInt32Number BaseOffset;

    BaseOffset = io ->Tell(io) - sizeof(_cmsTagBase);

    if (!_cmsReadUInt16Number(io, &InputChans)) return NULL;
    if (!_cmsReadUInt16Number(io, &OutputChans)) return NULL;

    NewLUT = cmsPipelineAlloc(self ->ContextID, InputChans, OutputChans);
    if (NewLUT == NULL) return NULL;

    if (!_cmsReadUInt32Number(io, &ElementCount)) return NULL;

    if (!ReadPositionTable(self, io, ElementCount, BaseOffset, NewLUT, ReadMPEElem)) {
        if (NewLUT != NULL) cmsPipelineFree(NewLUT);
        *nItems = 0;
        return NULL;
    }

    *nItems = 1;
    return NewLUT;

    cmsUNUSED_PARAMETER(SizeOfTag);
}
