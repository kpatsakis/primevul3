Mat_Create4(const char* matname)
{
    FILE *fp = NULL;
    mat_t *mat = NULL;

#if defined(_WIN32) && defined(_MSC_VER)
    wchar_t* wname = utf82u(matname);
    if ( NULL != wname ) {
        fp = _wfopen(wname, L"w+b");
        free(wname);
    }
#else
    fp = fopen(matname, "w+b");
#endif
    if ( !fp )
        return NULL;

    mat = (mat_t*)malloc(sizeof(*mat));
    if ( NULL == mat ) {
        fclose(fp);
        Mat_Critical("Couldn't allocate memory for the MAT file");
        return NULL;
    }

    mat->fp            = fp;
    mat->header        = NULL;
    mat->subsys_offset = NULL;
    mat->filename      = strdup_printf("%s",matname);
    mat->version       = MAT_FT_MAT4;
    mat->byteswap      = 0;
    mat->mode          = 0;
    mat->bof           = 0;
    mat->next_index    = 0;
    mat->num_datasets  = 0;
#if defined(MAT73) && MAT73
    mat->refs_id       = -1;
#endif
    mat->dir           = NULL;

    Mat_Rewind(mat);

    return mat;
}
