gplotGenDataFiles(GPLOT  *gplot)
{
char    *plotdata, *dataname;
l_int32  i, nplots;
FILE    *fp;

    PROCNAME("gplotGenDataFiles");

    if (!gplot)
        return ERROR_INT("gplot not defined", procName, 1);

    nplots = sarrayGetCount(gplot->datanames);
    for (i = 0; i < nplots; i++) {
        plotdata = sarrayGetString(gplot->plotdata, i, L_NOCOPY);
        dataname = sarrayGetString(gplot->datanames, i, L_NOCOPY);
        if ((fp = fopen(dataname, "w")) == NULL)
            return ERROR_INT("datafile stream not opened", procName, 1);
        fwrite(plotdata, 1, strlen(plotdata), fp);
        fclose(fp);
    }

    return 0;
}
