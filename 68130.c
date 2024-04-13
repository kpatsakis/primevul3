gplotSimpleXY1(NUMA        *nax,
               NUMA        *nay,
               l_int32      plotstyle,
               l_int32      outformat,
               const char  *outroot,
               const char  *title)
{
GPLOT  *gplot;

    PROCNAME("gplotSimpleXY1");

    if (!nay)
        return ERROR_INT("nay not defined", procName, 1);
    if (plotstyle < 0 || plotstyle >= NUM_GPLOT_STYLES)
        return ERROR_INT("invalid plotstyle", procName, 1);
    if (outformat != GPLOT_PNG && outformat != GPLOT_PS &&
        outformat != GPLOT_EPS && outformat != GPLOT_LATEX)
        return ERROR_INT("invalid outformat", procName, 1);
    if (!outroot)
        return ERROR_INT("outroot not specified", procName, 1);

    if ((gplot = gplotCreate(outroot, outformat, title, NULL, NULL)) == 0)
        return ERROR_INT("gplot not made", procName, 1);
    gplotAddPlot(gplot, nax, nay, plotstyle, NULL);
    gplotMakeOutput(gplot);
    gplotDestroy(&gplot);
    return 0;
}
