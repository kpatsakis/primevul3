cmsBool RegisterTypesPlugin(cmsContext id, cmsPluginBase* Data, _cmsMemoryClient pos)
{
    cmsPluginTagType* Plugin = (cmsPluginTagType*) Data;
    _cmsTagTypePluginChunkType* ctx = ( _cmsTagTypePluginChunkType*) _cmsContextGetClientChunk(id, pos);
    _cmsTagTypeLinkedList *pt;

    if (Data == NULL) {

        ctx ->TagTypes = NULL;
        return TRUE;
    }

    pt = (_cmsTagTypeLinkedList*) _cmsPluginMalloc(id, sizeof(_cmsTagTypeLinkedList));
    if (pt == NULL) return FALSE;

    pt ->Handler   = Plugin ->Handler;
    pt ->Next      = ctx ->TagTypes;

    ctx ->TagTypes = pt;
     
    return TRUE;
}
