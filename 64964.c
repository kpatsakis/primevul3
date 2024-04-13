void WriteDataFormat(SAVESTREAM* fp, cmsIT8* it8)
{
    int i, nSamples;
    TABLE* t = GetTable(it8);

    if (!t -> DataFormat) return;

       WriteStr(fp, "BEGIN_DATA_FORMAT\n");
       WriteStr(fp, " ");
       nSamples = atoi(cmsIT8GetProperty(it8, "NUMBER_OF_FIELDS"));

       for (i = 0; i < nSamples; i++) {

              WriteStr(fp, t->DataFormat[i]);
              WriteStr(fp, ((i == (nSamples-1)) ? "\n" : "\t"));
          }

       WriteStr (fp, "END_DATA_FORMAT\n");
}
