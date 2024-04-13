cmsBool CMSEXPORT cmsIT8SaveToFile(cmsHANDLE hIT8, const char* cFileName)
{
    SAVESTREAM sd;
    cmsUInt32Number i;
    cmsIT8* it8 = (cmsIT8*) hIT8;

    memset(&sd, 0, sizeof(sd));

    sd.stream = fopen(cFileName, "wt");
    if (!sd.stream) return FALSE;

    for (i=0; i < it8 ->TablesCount; i++) {

            cmsIT8SetTable(hIT8, i);
            WriteHeader(it8, &sd);
            WriteDataFormat(&sd, it8);
            WriteData(&sd, it8);
    }

    if (fclose(sd.stream) != 0) return FALSE;

    return TRUE;
}
