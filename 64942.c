void CookPointers(cmsIT8* it8)
{
    int idField, i;
    char* Fld;
    cmsUInt32Number j;
    cmsUInt32Number nOldTable = it8 ->nTable;

    for (j=0; j < it8 ->TablesCount; j++) {

    TABLE* t = it8 ->Tab + j;

    t -> SampleID = 0;
    it8 ->nTable = j;

    for (idField = 0; idField < t -> nSamples; idField++)
    {
        if (t ->DataFormat == NULL){
            SynError(it8, "Undefined DATA_FORMAT");
            return;
        }

        Fld = t->DataFormat[idField];
        if (!Fld) continue;


        if (cmsstrcasecmp(Fld, "SAMPLE_ID") == 0) {

            t -> SampleID = idField;

            for (i=0; i < t -> nPatches; i++) {

                char *Data = GetData(it8, i, idField);
                if (Data) {
                    char Buffer[256];

                    strncpy(Buffer, Data, 255);
                    Buffer[255] = 0;

                    if (strlen(Buffer) <= strlen(Data))
                        strcpy(Data, Buffer);
                    else
                        SetData(it8, i, idField, Buffer);

                }
            }

        }


        if ((cmsstrcasecmp(Fld, "LABEL") == 0) || Fld[0] == '$' ) {

                    for (i=0; i < t -> nPatches; i++) {

                            char *Label = GetData(it8, i, idField);

                            if (Label) {

                                cmsUInt32Number k;


                                for (k=0; k < it8 ->TablesCount; k++) {

                                    TABLE* Table = it8 ->Tab + k;
                                    KEYVALUE* p;

                                    if (IsAvailableOnList(Table->HeaderList, Label, NULL, &p)) {

                                        char Buffer[256];

                                        char *Type  = p ->Value;
                                        int  nTable = (int) k;

                                        snprintf(Buffer, 255, "%s %d %s", Label, nTable, Type );

                                        SetData(it8, i, idField, Buffer);
                                    }
                                }


                            }

                    }


        }

    }
    }

    it8 ->nTable = nOldTable;
}
