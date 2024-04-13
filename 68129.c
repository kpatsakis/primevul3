gplotSimpleN(NUMAA       *naa,
             l_int32      outformat,
             const char  *outroot,
             const char  *title)
{
    return gplotSimpleXYN(NULL, naa, GPLOT_LINES, outformat, outroot, title);
}
