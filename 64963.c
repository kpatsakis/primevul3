void WriteData(SAVESTREAM* fp, cmsIT8* it8)
{
       int  i, j;
       TABLE* t = GetTable(it8);

       if (!t->Data) return;

       WriteStr (fp, "BEGIN_DATA\n");

       t->nPatches = atoi(cmsIT8GetProperty(it8, "NUMBER_OF_SETS"));

       for (i = 0; i < t-> nPatches; i++) {

              WriteStr(fp, " ");

              for (j = 0; j < t->nSamples; j++) {

                     char *ptr = t->Data[i*t->nSamples+j];

                     if (ptr == NULL) WriteStr(fp, "\"\"");
                     else {

                         if (strchr(ptr, ' ') != NULL) {

                             WriteStr(fp, "\"");
                             WriteStr(fp, ptr);
                             WriteStr(fp, "\"");
                         }
                         else
                            WriteStr(fp, ptr);
                     }

                     WriteStr(fp, ((j == (t->nSamples-1)) ? "\n" : "\t"));
              }
       }
       WriteStr (fp, "END_DATA\n");
}
