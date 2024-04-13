void *Type_ProfileSequenceId_Read(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number* nItems, cmsUInt32Number SizeOfTag)
{
    cmsSEQ* OutSeq;
    cmsUInt32Number Count;
    cmsUInt32Number BaseOffset;

    *nItems = 0;

    BaseOffset = io ->Tell(io) - sizeof(_cmsTagBase);

    if (!_cmsReadUInt32Number(io, &Count)) return NULL;
    SizeOfTag -= sizeof(cmsUInt32Number);

    OutSeq = cmsAllocProfileSequenceDescription(self ->ContextID, Count);
    if (OutSeq == NULL) return NULL;


    if (!ReadPositionTable(self, io, Count, BaseOffset, OutSeq, ReadSeqID)) {

        cmsFreeProfileSequenceDescription(OutSeq);
        return NULL;
    }

    *nItems = 1;
    return OutSeq;

}
