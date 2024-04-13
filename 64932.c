KEYVALUE* AddAvailableSampleID(cmsIT8* it8, const char* Key)
{
    return AddToList(it8, &it8->ValidSampleID, Key, NULL, NULL, WRITE_UNCOOKED);
}
