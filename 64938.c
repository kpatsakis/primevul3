void AllocateDataFormat(cmsIT8* it8)
{
    TABLE* t = GetTable(it8);

    if (t -> DataFormat) return;    // Already allocated

    t -> nSamples  = (int) cmsIT8GetPropertyDbl(it8, "NUMBER_OF_FIELDS");

    if (t -> nSamples <= 0) {

        SynError(it8, "AllocateDataFormat: Unknown NUMBER_OF_FIELDS");
        t -> nSamples = 10;
        }

    t -> DataFormat = (char**) AllocChunk (it8, ((cmsUInt32Number) t->nSamples + 1) * sizeof(char *));
    if (t->DataFormat == NULL) {

        SynError(it8, "AllocateDataFormat: Unable to allocate dataFormat array");
    }

}
