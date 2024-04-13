gplotSimple2(NUMA        *na1,
             NUMA        *na2,
             l_int32      outformat,
             const char  *outroot,
             const char  *title)
{
    return gplotSimpleXY2(NULL, na1, na2, GPLOT_LINES,
                          outformat, outroot, title);
}
