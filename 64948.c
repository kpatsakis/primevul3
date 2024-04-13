cmsBool HeaderSection(cmsIT8* it8)
{
    char VarName[MAXID];
    char Buffer[MAXSTR];
    KEYVALUE* Key;

        while (it8->sy != SEOF &&
               it8->sy != SSYNERROR &&
               it8->sy != SBEGIN_DATA_FORMAT &&
               it8->sy != SBEGIN_DATA) {


        switch (it8 -> sy) {

        case SKEYWORD:
                InSymbol(it8);
                if (!GetVal(it8, Buffer, MAXSTR-1, "Keyword expected")) return FALSE;
                if (!AddAvailableProperty(it8, Buffer, WRITE_UNCOOKED)) return FALSE;
                InSymbol(it8);
                break;


        case SDATA_FORMAT_ID:
                InSymbol(it8);
                if (!GetVal(it8, Buffer, MAXSTR-1, "Keyword expected")) return FALSE;
                if (!AddAvailableSampleID(it8, Buffer)) return FALSE;
                InSymbol(it8);
                break;


        case SIDENT:
            strncpy(VarName, it8->id, MAXID - 1);
            VarName[MAXID - 1] = 0;

            if (!IsAvailableOnList(it8->ValidKeywords, VarName, NULL, &Key)) {

#ifdef CMS_STRICT_CGATS
                return SynError(it8, "Undefined keyword '%s'", VarName);
#else
                Key = AddAvailableProperty(it8, VarName, WRITE_UNCOOKED);
                if (Key == NULL) return FALSE;
#endif
            }

            InSymbol(it8);
            if (!GetVal(it8, Buffer, MAXSTR - 1, "Property data expected")) return FALSE;

            if (Key->WriteAs != WRITE_PAIR) {
                AddToList(it8, &GetTable(it8)->HeaderList, VarName, NULL, Buffer,
                    (it8->sy == SSTRING) ? WRITE_STRINGIFY : WRITE_UNCOOKED);
            }
            else {
                const char *Subkey;
                char *Nextkey;
                if (it8->sy != SSTRING)
                    return SynError(it8, "Invalid value '%s' for property '%s'.", Buffer, VarName);

                for (Subkey = Buffer; Subkey != NULL; Subkey = Nextkey)
                {
                    char *Value, *temp;

                    Nextkey = (char*)strchr(Subkey, ';');
                    if (Nextkey)
                        *Nextkey++ = '\0';

                    Value = (char*)strrchr(Subkey, ',');
                    if (Value == NULL)
                        return SynError(it8, "Invalid value for property '%s'.", VarName);

                    temp = Value++;
                    do *temp-- = '\0'; while (temp >= Subkey && *temp == ' ');

                    temp = Value + strlen(Value) - 1;
                    while (*temp == ' ') *temp-- = '\0';

                    Subkey += strspn(Subkey, " ");
                    Value += strspn(Value, " ");

                    if (Subkey[0] == 0 || Value[0] == 0)
                        return SynError(it8, "Invalid value for property '%s'.", VarName);
                    AddToList(it8, &GetTable(it8)->HeaderList, VarName, Subkey, Value, WRITE_PAIR);
                }
            }

            InSymbol(it8);
            break;


        case SEOLN: break;

        default:
                return SynError(it8, "expected keyword or identifier");
        }

    SkipEOLN(it8);
    }

    return TRUE;

}
