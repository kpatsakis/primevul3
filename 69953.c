Mat_VarWrite4(mat_t *mat,matvar_t *matvar)
{
    typedef struct {
        mat_int32_t type;
        mat_int32_t mrows;
        mat_int32_t ncols;
        mat_int32_t imagf;
        mat_int32_t namelen;
    } Fmatrix;

    mat_int32_t nelems = 1, i;
    Fmatrix x;

    if ( NULL == mat || NULL == matvar || NULL == matvar->name || matvar->rank != 2 )
        return -1;

    switch ( matvar->data_type ) {
        case MAT_T_DOUBLE:
            x.type = 0;
            break;
        case MAT_T_SINGLE:
            x.type = 10;
            break;
        case MAT_T_INT32:
            x.type = 20;
            break;
        case MAT_T_INT16:
            x.type = 30;
            break;
        case MAT_T_UINT16:
            x.type = 40;
            break;
        case MAT_T_UINT8:
            x.type = 50;
            break;
        default:
            return 2;
    }

#if defined(__GLIBC__)
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#elif (__BYTE_ORDER == __BIG_ENDIAN)
    x.type += 1000;
#else
    return -1;
#endif
#elif defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)
    x.type += 1000;
#elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)
#elif defined(__sparc) || defined(__sparc__) || defined(_POWER) || defined(__powerpc__) || \
      defined(__ppc__) || defined(__hpux) || defined(_MIPSEB) || defined(_POWER) || defined(__s390__)
    x.type += 1000;
#elif defined(__i386__) || defined(__alpha__) || defined(__ia64) || defined(__ia64__) || \
      defined(_M_IX86) || defined(_M_IA64) || defined(_M_ALPHA) || defined(__amd64) || \
      defined(__amd64__) || defined(_M_AMD64) || defined(__x86_64) || defined(__x86_64__) || \
      defined(_M_X64) || defined(__bfin__)
#else
    return -1;
#endif

    x.namelen = (mat_int32_t)strlen(matvar->name) + 1;

    /* FIXME: SEEK_END is not Guaranteed by the C standard */
    (void)fseek((FILE*)mat->fp,0,SEEK_END);         /* Always write at end of file */

    switch ( matvar->class_type ) {
        case MAT_C_CHAR:
            x.type++;
            /* Fall through */
        case MAT_C_DOUBLE:
        case MAT_C_SINGLE:
        case MAT_C_INT32:
        case MAT_C_INT16:
        case MAT_C_UINT16:
        case MAT_C_UINT8:
            for ( i = 0; i < matvar->rank; i++ ) {
                mat_int32_t dim;
                dim = (mat_int32_t)matvar->dims[i];
                nelems *= dim;
            }

            x.mrows = (mat_int32_t)matvar->dims[0];
            x.ncols = (mat_int32_t)matvar->dims[1];
            x.imagf = matvar->isComplex ? 1 : 0;
            fwrite(&x, sizeof(Fmatrix), 1, (FILE*)mat->fp);
            fwrite(matvar->name, sizeof(char), x.namelen, (FILE*)mat->fp);
            if ( matvar->isComplex ) {
                mat_complex_split_t *complex_data;

                complex_data = (mat_complex_split_t*)matvar->data;
                fwrite(complex_data->Re, matvar->data_size, nelems, (FILE*)mat->fp);
                fwrite(complex_data->Im, matvar->data_size, nelems, (FILE*)mat->fp);
            }
            else {
                fwrite(matvar->data, matvar->data_size, nelems, (FILE*)mat->fp);
            }
            break;
        case MAT_C_SPARSE:
        {
            mat_sparse_t* sparse;
            double tmp;
            int j;
            size_t stride = Mat_SizeOf(matvar->data_type);
#if !defined(EXTENDED_SPARSE)
            if ( MAT_T_DOUBLE != matvar->data_type )
                break;
#endif

            sparse = (mat_sparse_t*)matvar->data;
            x.type += 2;
            x.mrows = sparse->njc > 0 ? sparse->jc[sparse->njc - 1] + 1 : 1;
            x.ncols = matvar->isComplex ? 4 : 3;
            x.imagf = 0;

            fwrite(&x, sizeof(Fmatrix), 1, (FILE*)mat->fp);
            fwrite(matvar->name, sizeof(char), x.namelen, (FILE*)mat->fp);

            for ( i = 0; i < sparse->njc - 1; i++ ) {
                for ( j = sparse->jc[i];
                      j < sparse->jc[i + 1] && j < sparse->ndata; j++ ) {
                    tmp = sparse->ir[j] + 1;
                    fwrite(&tmp, sizeof(double), 1, (FILE*)mat->fp);
                }
            }
            tmp = (double)matvar->dims[0];
            fwrite(&tmp, sizeof(double), 1, (FILE*)mat->fp);
            for ( i = 0; i < sparse->njc - 1; i++ ) {
                for ( j = sparse->jc[i];
                      j < sparse->jc[i + 1] && j < sparse->ndata; j++ ) {
                    tmp = i + 1;
                    fwrite(&tmp, sizeof(double), 1, (FILE*)mat->fp);
                }
            }
            tmp = (double)matvar->dims[1];
            fwrite(&tmp, sizeof(double), 1, (FILE*)mat->fp);
            tmp = 0.;
            if ( matvar->isComplex ) {
                mat_complex_split_t *complex_data;
                char* re, *im;

                complex_data = (mat_complex_split_t*)sparse->data;
                re = (char*)complex_data->Re;
                im = (char*)complex_data->Im;
                for ( i = 0; i < sparse->njc - 1; i++ ) {
                    for ( j = sparse->jc[i];
                          j < sparse->jc[i + 1] && j < sparse->ndata; j++ ) {
                        fwrite(re + j*stride, stride, 1, (FILE*)mat->fp);
                    }
                }
                fwrite(&tmp, stride, 1, (FILE*)mat->fp);
                for ( i = 0; i < sparse->njc - 1; i++ ) {
                    for ( j = sparse->jc[i];
                          j < sparse->jc[i + 1] && j < sparse->ndata; j++ ) {
                        fwrite(im + j*stride, stride, 1, (FILE*)mat->fp);
                    }
                }
            } else {
                char *data = (char*)sparse->data;
                for ( i = 0; i < sparse->njc - 1; i++ ) {
                    for ( j = sparse->jc[i];
                          j < sparse->jc[i + 1] && j < sparse->ndata; j++ ) {
                        fwrite(data + j*stride, stride, 1, (FILE*)mat->fp);
                    }
                }
            }
            fwrite(&tmp, stride, 1, (FILE*)mat->fp);
            break;
        }
        default:
            break;
    }

    return 0;
}
