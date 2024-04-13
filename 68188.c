selaCreateFromFile(const char  *filename)
{
char    *filestr, *line;
l_int32  i, n, first, last, nsel, insel;
size_t   nbytes;
NUMA    *nafirst, *nalast;
SARRAY  *sa;
SEL     *sel;
SELA    *sela;

    PROCNAME("selaCreateFromFile");

    if (!filename)
        return (SELA *)ERROR_PTR("filename not defined", procName, NULL);

    filestr = (char *)l_binaryRead(filename, &nbytes);
    sa = sarrayCreateLinesFromString(filestr, 1);
    LEPT_FREE(filestr);
    n = sarrayGetCount(sa);
    sela = selaCreate(0);

        /* Find the start and end lines for each Sel.
         * We allow the "blank" lines to be null strings or
         * to have standard whitespace (' ','\t',\'n') or be '#'. */
    nafirst = numaCreate(0);
    nalast = numaCreate(0);
    insel = FALSE;
    for (i = 0; i < n; i++) {
        line = sarrayGetString(sa, i, L_NOCOPY);
        if (!insel &&
            (line[0] != '\0' && line[0] != ' ' &&
             line[0] != '\t' && line[0] != '\n' && line[0] != '#')) {
            numaAddNumber(nafirst, i);
            insel = TRUE;
            continue;
        }
        if (insel &&
            (line[0] == '\0' || line[0] == ' ' ||
             line[0] == '\t' || line[0] == '\n' || line[0] == '#')) {
            numaAddNumber(nalast, i - 1);
            insel = FALSE;
            continue;
        }
    }
    if (insel)  /* fell off the end of the file */
        numaAddNumber(nalast, n - 1);

        /* Extract sels */
    nsel = numaGetCount(nafirst);
    for (i = 0; i < nsel; i++) {
        numaGetIValue(nafirst, i, &first);
        numaGetIValue(nalast, i, &last);
        if ((sel = selCreateFromSArray(sa, first, last)) == NULL) {
            fprintf(stderr, "Error reading sel from %d to %d\n", first, last);
            selaDestroy(&sela);
            sarrayDestroy(&sa);
            numaDestroy(&nafirst);
            numaDestroy(&nalast);
            return (SELA *)ERROR_PTR("bad sela file", procName, NULL);
        }
        selaAddSel(sela, sel, NULL, 0);
    }

    numaDestroy(&nafirst);
    numaDestroy(&nalast);
    sarrayDestroy(&sa);
    return sela;
}
