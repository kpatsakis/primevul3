cmsBool ReadMPEElem(struct _cms_typehandler_struct* self,
                    cmsIOHANDLER* io,
                    void* Cargo,
                    cmsUInt32Number n,
                    cmsUInt32Number SizeOfTag)
{
    cmsStageSignature ElementSig;
    cmsTagTypeHandler* TypeHandler;
    cmsUInt32Number nItems;
    cmsPipeline *NewLUT = (cmsPipeline *) Cargo;
    _cmsTagTypePluginChunkType* MPETypePluginChunk  = ( _cmsTagTypePluginChunkType*) _cmsContextGetClientChunk(self->ContextID, MPEPlugin);


    if (!_cmsReadUInt32Number(io, (cmsUInt32Number*) &ElementSig)) return FALSE;

    if (!_cmsReadUInt32Number(io, NULL)) return FALSE;

    TypeHandler = GetHandler((cmsTagTypeSignature) ElementSig, MPETypePluginChunk ->TagTypes, SupportedMPEtypes);
    if (TypeHandler == NULL)  {

        char String[5];

        _cmsTagSignature2String(String, (cmsTagSignature) ElementSig);

        cmsSignalError(self ->ContextID, cmsERROR_UNKNOWN_EXTENSION, "Unknown MPE type '%s' found.", String);
        return FALSE;
    }

    if (TypeHandler ->ReadPtr != NULL) {

        if (!cmsPipelineInsertStage(NewLUT, cmsAT_END, (cmsStage*) TypeHandler ->ReadPtr(self, io, &nItems, SizeOfTag)))
            return FALSE;
    }

    return TRUE;

    cmsUNUSED_PARAMETER(SizeOfTag);
    cmsUNUSED_PARAMETER(n);
}
