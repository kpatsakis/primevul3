cmsBool SetData(cmsIT8* it8, int nSet, int nField, const char *Val)
{
    TABLE* t = GetTable(it8);

    if (!t->Data)
        AllocateDataSet(it8);

    if (!t->Data) return FALSE;

    if (nSet > t -> nPatches || nSet < 0) {

            return SynError(it8, "Patch %d out of range, there are %d patches", nSet, t -> nPatches);
    }

    if (nField > t ->nSamples || nField < 0) {
            return SynError(it8, "Sample %d out of range, there are %d samples", nField, t ->nSamples);

    }

    t->Data [nSet * t -> nSamples + nField] = AllocString(it8, Val);
    return TRUE;
}
