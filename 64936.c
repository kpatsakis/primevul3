char *AllocString(cmsIT8* it8, const char* str)
{
    cmsUInt32Number Size = (cmsUInt32Number) strlen(str)+1;
    char *ptr;


    ptr = (char *) AllocChunk(it8, Size);
    if (ptr) strncpy (ptr, str, Size-1);

    return ptr;
}
