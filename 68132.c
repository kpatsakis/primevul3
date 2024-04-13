gplotSimpleXYN(NUMA        *nax,
               NUMAA       *naay,
               l_int32      plotstyle,
               l_int32      outformat,
               const char  *outroot,
               const char  *title)
{
l_int32  i, n;
GPLOT   *gplot;
NUMA    *nay;

    PROCNAME("gplotSimpleXYN");

    if (!naay)
        return ERROR_INT("naay not defined", procName, 1);
    if ((n = numaaGetCount(naay)) == 0)
        return ERROR_INT("no numa in array", procName, 1);
    if (plotstyle < 0 || plotstyle >= NUM_GPLOT_STYLES)
        return ERROR_INT("invalid plotstyle", procName, 1);
    if (outformat != GPLOT_PNG && outformat != GPLOT_PS &&
        outformat != GPLOT_EPS && outformat != GPLOT_LATEX)
        return ERROR_INT("invalid outformat", procName, 1);
    if (!outroot)
        return ERROR_INT("outroot not specified", procName, 1);

    if ((gplot = gplotCreate(outroot, outformat, title, NULL, NULL)) == 0)
        return ERROR_INT("gplot not made", procName, 1);
    for (i = 0; i < n; i++) {
        nay = numaaGetNuma(naay, i, L_CLONE);
        gplotAddPlot(gplot, nax, nay, plotstyle, NULL);
        numaDestroy(&nay);
    }
    gplotMakeOutput(gplot);
    gplotDestroy(&gplot);
    return 0;
}
