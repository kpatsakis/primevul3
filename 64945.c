char* GetData(cmsIT8* it8, int nSet, int nField)
{
    TABLE* t = GetTable(it8);
    int nSamples    = t -> nSamples;
    int nPatches    = t -> nPatches;

    if (nSet >= nPatches || nField >= nSamples)
        return NULL;

    if (!t->Data) return NULL;
    return t->Data [nSet * nSamples + nField];
}
