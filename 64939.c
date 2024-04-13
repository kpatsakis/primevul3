SYMBOL BinSrchKey(const char *id)
{
    int l = 1;
    int r = NUMKEYS;
    int x, res;

    while (r >= l)
    {
        x = (l+r)/2;
        res = cmsstrcasecmp(id, TabKeys[x-1].id);
        if (res == 0) return TabKeys[x-1].sy;
        if (res < 0) r = x - 1;
        else l = x + 1;
    }

    return SUNDEFINED;
}
