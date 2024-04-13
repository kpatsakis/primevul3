void *Type_LUT8_Read(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number* nItems, cmsUInt32Number SizeOfTag)
{
    cmsUInt8Number InputChannels, OutputChannels, CLUTpoints;
    cmsUInt8Number* Temp = NULL;
    cmsPipeline* NewLUT = NULL;
    cmsUInt32Number nTabSize, i;
    cmsFloat64Number Matrix[3*3];

    *nItems = 0;

    if (!_cmsReadUInt8Number(io, &InputChannels)) goto Error;
    if (!_cmsReadUInt8Number(io, &OutputChannels)) goto Error;
    if (!_cmsReadUInt8Number(io, &CLUTpoints)) goto Error;

     if (CLUTpoints == 1) goto Error; // Impossible value, 0 for no CLUT and then 2 at least

    if (!_cmsReadUInt8Number(io, NULL)) goto Error;

    if (InputChannels > cmsMAXCHANNELS)  goto Error;
    if (OutputChannels > cmsMAXCHANNELS) goto Error;

    NewLUT = cmsPipelineAlloc(self ->ContextID, InputChannels, OutputChannels);
    if (NewLUT == NULL) goto Error;

    if (!_cmsRead15Fixed16Number(io,  &Matrix[0])) goto Error;
    if (!_cmsRead15Fixed16Number(io,  &Matrix[1])) goto Error;
    if (!_cmsRead15Fixed16Number(io,  &Matrix[2])) goto Error;
    if (!_cmsRead15Fixed16Number(io,  &Matrix[3])) goto Error;
    if (!_cmsRead15Fixed16Number(io,  &Matrix[4])) goto Error;
    if (!_cmsRead15Fixed16Number(io,  &Matrix[5])) goto Error;
    if (!_cmsRead15Fixed16Number(io,  &Matrix[6])) goto Error;
    if (!_cmsRead15Fixed16Number(io,  &Matrix[7])) goto Error;
    if (!_cmsRead15Fixed16Number(io,  &Matrix[8])) goto Error;


    if ((InputChannels == 3) && !_cmsMAT3isIdentity((cmsMAT3*) Matrix)) {

        if (!cmsPipelineInsertStage(NewLUT, cmsAT_BEGIN, cmsStageAllocMatrix(self ->ContextID, 3, 3, Matrix, NULL)))
            goto Error;
    }

    if (!Read8bitTables(self ->ContextID, io,  NewLUT, InputChannels)) goto Error;

    nTabSize = uipow(OutputChannels, CLUTpoints, InputChannels);
    if (nTabSize == (cmsUInt32Number) -1) goto Error;
    if (nTabSize > 0) {

        cmsUInt16Number *PtrW, *T;
       
        PtrW = T  = (cmsUInt16Number*) _cmsCalloc(self ->ContextID, nTabSize, sizeof(cmsUInt16Number));
        if (T  == NULL) goto Error;

        Temp = (cmsUInt8Number*) _cmsMalloc(self ->ContextID, nTabSize);
        if (Temp == NULL) {
            _cmsFree(self ->ContextID, T);
            goto Error;
        }

        if (io ->Read(io, Temp, nTabSize, 1) != 1) {
            _cmsFree(self ->ContextID, T);
            _cmsFree(self ->ContextID, Temp);
            goto Error;
        }

        for (i = 0; i < nTabSize; i++) {

            *PtrW++ = FROM_8_TO_16(Temp[i]);
        }
        _cmsFree(self ->ContextID, Temp);
        Temp = NULL;

        if (!cmsPipelineInsertStage(NewLUT, cmsAT_END, cmsStageAllocCLut16bit(self ->ContextID, CLUTpoints, InputChannels, OutputChannels, T)))
            goto Error;
        _cmsFree(self ->ContextID, T);
    }


    if (!Read8bitTables(self ->ContextID, io,  NewLUT, OutputChannels)) goto Error;

    *nItems = 1;
    return NewLUT;

Error:
    if (NewLUT != NULL) cmsPipelineFree(NewLUT);
    return NULL;

    cmsUNUSED_PARAMETER(SizeOfTag);
}
