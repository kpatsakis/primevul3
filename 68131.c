gplotSimpleXY2(NUMA        *nax,
               NUMA        *nay1,
               NUMA        *nay2,
               l_int32      plotstyle,
               l_int32      outformat,
               const char  *outroot,
               const char  *title)
{
GPLOT  *gplot;

    PROCNAME("gplotSimpleXY2");

    if (!nay1 || !nay2)
        return ERROR_INT("nay1 and nay2 not both defined", procName, 1);
    if (plotstyle < 0 || plotstyle >= NUM_GPLOT_STYLES)
        return ERROR_INT("invalid plotstyle", procName, 1);
    if (outformat != GPLOT_PNG && outformat != GPLOT_PS &&
        outformat != GPLOT_EPS && outformat != GPLOT_LATEX)
        return ERROR_INT("invalid outformat", procName, 1);
    if (!outroot)
        return ERROR_INT("outroot not specified", procName, 1);

    if ((gplot = gplotCreate(outroot, outformat, title, NULL, NULL)) == 0)
        return ERROR_INT("gplot not made", procName, 1);
    gplotAddPlot(gplot, nax, nay1, plotstyle, NULL);
    gplotAddPlot(gplot, nax, nay2, plotstyle, NULL);
    gplotMakeOutput(gplot);
    gplotDestroy(&gplot);
    return 0;
}
