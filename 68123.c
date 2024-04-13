int main(int    argc,
         char **argv)
{
char        *dirin, *dirout, *rootname;
l_int32      thumbwidth, viewwidth;
static char  mainName[] = "htmlviewer";

    if (argc != 6)
        return ERROR_INT(
            " Syntax:  htmlviewer dirin dirout rootname thumbwidth viewwidth",
             mainName, 1);

    dirin = argv[1];
    dirout = argv[2];
    rootname = argv[3];
    thumbwidth = atoi(argv[4]);
    viewwidth = atoi(argv[5]);
    pixHtmlViewer(dirin, dirout, rootname, thumbwidth, viewwidth);
    return 0;
}
