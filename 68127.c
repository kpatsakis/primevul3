gplotSimple1(NUMA        *na,
             l_int32      outformat,
             const char  *outroot,
             const char  *title)
{
    return gplotSimpleXY1(NULL, na, GPLOT_LINES, outformat, outroot, title);
}
