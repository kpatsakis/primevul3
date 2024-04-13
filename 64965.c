void WriteHeader(cmsIT8* it8, SAVESTREAM* fp)
{
    KEYVALUE* p;
    TABLE* t = GetTable(it8);

    WriteStr(fp, t->SheetType);
    WriteStr(fp, "\n");

    for (p = t->HeaderList; (p != NULL); p = p->Next)
    {
        if (*p ->Keyword == '#') {

            char* Pt;

            WriteStr(fp, "#\n# ");
            for (Pt = p ->Value; *Pt; Pt++) {


                Writef(fp, "%c", *Pt);

                if (*Pt == '\n') {
                    WriteStr(fp, "# ");
                }
            }

            WriteStr(fp, "\n#\n");
            continue;
        }


        if (!IsAvailableOnList(it8-> ValidKeywords, p->Keyword, NULL, NULL)) {

#ifdef CMS_STRICT_CGATS
            WriteStr(fp, "KEYWORD\t\"");
            WriteStr(fp, p->Keyword);
            WriteStr(fp, "\"\n");
#endif

            AddAvailableProperty(it8, p->Keyword, WRITE_UNCOOKED);
        }

        WriteStr(fp, p->Keyword);
        if (p->Value) {

            switch (p ->WriteAs) {

            case WRITE_UNCOOKED:
                    Writef(fp, "\t%s", p ->Value);
                    break;

            case WRITE_STRINGIFY:
                    Writef(fp, "\t\"%s\"", p->Value );
                    break;

            case WRITE_HEXADECIMAL:
                    Writef(fp, "\t0x%X", atoi(p ->Value));
                    break;

            case WRITE_BINARY:
                    Writef(fp, "\t0x%B", atoi(p ->Value));
                    break;

            case WRITE_PAIR:
                    Writef(fp, "\t\"%s,%s\"", p->Subkey, p->Value);
                    break;

            default: SynError(it8, "Unknown write mode %d", p ->WriteAs);
                     return;
            }
        }

        WriteStr (fp, "\n");
    }

}
