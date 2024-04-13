gplotDestroy(GPLOT  **pgplot)
{
GPLOT  *gplot;

    PROCNAME("gplotDestroy");

    if (pgplot == NULL) {
        L_WARNING("ptr address is null!\n", procName);
        return;
    }

    if ((gplot = *pgplot) == NULL)
        return;

    LEPT_FREE(gplot->rootname);
    LEPT_FREE(gplot->cmdname);
    sarrayDestroy(&gplot->cmddata);
    sarrayDestroy(&gplot->datanames);
    sarrayDestroy(&gplot->plotdata);
    sarrayDestroy(&gplot->plottitles);
    numaDestroy(&gplot->plotstyles);
    LEPT_FREE(gplot->outname);
    if (gplot->title)
        LEPT_FREE(gplot->title);
    if (gplot->xlabel)
        LEPT_FREE(gplot->xlabel);
    if (gplot->ylabel)
        LEPT_FREE(gplot->ylabel);

    LEPT_FREE(gplot);
    *pgplot = NULL;
    return;
}
