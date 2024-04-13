cmsHANDLE  CMSEXPORT cmsIT8Alloc(cmsContext ContextID)
{
    cmsIT8* it8;
    cmsUInt32Number i;

    it8 = (cmsIT8*) _cmsMallocZero(ContextID, sizeof(cmsIT8));
    if (it8 == NULL) return NULL;

    AllocTable(it8);

    it8->MemoryBlock = NULL;
    it8->MemorySink  = NULL;

    it8 ->nTable = 0;

    it8->ContextID = ContextID;
    it8->Allocator.Used = 0;
    it8->Allocator.Block = NULL;
    it8->Allocator.BlockSize = 0;

    it8->ValidKeywords = NULL;
    it8->ValidSampleID = NULL;

    it8 -> sy = SUNDEFINED;
    it8 -> ch = ' ';
    it8 -> Source = NULL;
    it8 -> inum = 0;
    it8 -> dnum = 0.0;

    it8->FileStack[0] = (FILECTX*)AllocChunk(it8, sizeof(FILECTX));
    it8->IncludeSP   = 0;
    it8 -> lineno = 1;

    strcpy(it8->DoubleFormatter, DEFAULT_DBL_FORMAT);
    cmsIT8SetSheetType((cmsHANDLE) it8, "CGATS.17");


    for (i=0; i < NUMPREDEFINEDPROPS; i++)
            AddAvailableProperty(it8, PredefinedProperties[i].id, PredefinedProperties[i].as);

    for (i=0; i < NUMPREDEFINEDSAMPLEID; i++)
            AddAvailableSampleID(it8, PredefinedSampleID[i]);


   return (cmsHANDLE) it8;
}
