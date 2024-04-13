const char *GetDataFormat(cmsIT8* it8, int n)
{
    TABLE* t = GetTable(it8);

    if (t->DataFormat)
        return t->DataFormat[n];

    return NULL;
}
