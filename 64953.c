int LocateEmptyPatch(cmsIT8* it8)
{
    int i;
    const char *data;
    TABLE* t = GetTable(it8);

    for (i=0; i < t-> nPatches; i++) {

        data = GetData(it8, i, t->SampleID);

        if (data == NULL)
            return i;

    }

    return -1;
}
