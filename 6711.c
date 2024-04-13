const char * pgpValString(pgpValType type, uint8_t val)
{
    pgpValTbl tbl = pgpValTable(type);
    return (tbl != NULL) ? pgpValStr(tbl, val) : NULL;
}