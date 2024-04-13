static int pgpCurveByOid(const uint8_t *p, int l)
{
    uint8_t *curve;
    for (curve = curve_oids; *curve; curve += 2 + curve[1])
        if (l == (int)curve[1] && !memcmp(p, curve + 2, l))
            return (int)curve[0];
    return 0;
}