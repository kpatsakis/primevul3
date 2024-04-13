int LocatePatch(cmsIT8* it8, const char* cPatch)
{
    int i;
    const char *data;
    TABLE* t = GetTable(it8);

    for (i=0; i < t-> nPatches; i++) {

        data = GetData(it8, i, t->SampleID);

        if (data != NULL) {

                if (cmsstrcasecmp(data, cPatch) == 0)
                        return i;
                }
        }

        return -1;
}
