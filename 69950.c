Mat_VarRead4(mat_t *mat,matvar_t *matvar)
{
    int err;
    size_t nelems = 1;

    err = SafeMulDims(matvar, &nelems);
    if ( err ) {
        Mat_Critical("Integer multiplication overflow");
        return;
    }

    (void)fseek((FILE*)mat->fp,matvar->internal->datapos,SEEK_SET);

    switch ( matvar->class_type ) {
        case MAT_C_DOUBLE:
            matvar->data_size = sizeof(double);
            err = SafeMul(&matvar->nbytes, nelems, matvar->data_size);
            if ( err ) {
                Mat_Critical("Integer multiplication overflow");
                return;
            }

            if ( matvar->isComplex ) {
                mat_complex_split_t *complex_data = ComplexMalloc(matvar->nbytes);
                if ( NULL != complex_data ) {
                    matvar->data = complex_data;
                    ReadDoubleData(mat, (double*)complex_data->Re, matvar->data_type, nelems);
                    ReadDoubleData(mat, (double*)complex_data->Im, matvar->data_type, nelems);
                }
                else {
                    Mat_Critical("Couldn't allocate memory for the complex data");
                }
            } else {
                matvar->data = malloc(matvar->nbytes);
                if ( NULL != matvar->data ) {
                    ReadDoubleData(mat, (double*)matvar->data, matvar->data_type, nelems);
                }
                else {
                    Mat_Critical("Couldn't allocate memory for the data");
                }
            }
            /* Update data type to match format of matvar->data */
            matvar->data_type = MAT_T_DOUBLE;
            break;
        case MAT_C_CHAR:
            matvar->data_size = 1;
            matvar->nbytes = nelems;
            matvar->data = malloc(matvar->nbytes);
            if ( NULL != matvar->data ) {
                ReadUInt8Data(mat, (mat_uint8_t*)matvar->data, matvar->data_type, nelems);
            }
            else {
                Mat_Critical("Couldn't allocate memory for the data");
            }
            matvar->data_type = MAT_T_UINT8;
            break;
        case MAT_C_SPARSE:
            matvar->data_size = sizeof(mat_sparse_t);
            matvar->data      = malloc(matvar->data_size);
            if ( NULL != matvar->data ) {
                double tmp;
                int i;
                mat_sparse_t* sparse;
                long fpos;
                enum matio_types data_type = MAT_T_DOUBLE;

                /* matvar->dims[1] either is 3 for real or 4 for complex sparse */
                matvar->isComplex = matvar->dims[1] == 4 ? 1 : 0;
                sparse = (mat_sparse_t*)matvar->data;
                sparse->nir = matvar->dims[0] - 1;
                sparse->nzmax = sparse->nir;
                sparse->ir = (mat_int32_t*)malloc(sparse->nir*sizeof(mat_int32_t));
                if ( sparse->ir != NULL ) {
                    ReadInt32Data(mat, sparse->ir, data_type, sparse->nir);
                    for ( i = 0; i < sparse->nir; i++ )
                        sparse->ir[i] = sparse->ir[i] - 1;
                } else {
                    free(matvar->data);
                    matvar->data = NULL;
                    Mat_Critical("Couldn't allocate memory for the sparse row array");
                    return;
                }
                ReadDoubleData(mat, &tmp, data_type, 1);
                matvar->dims[0] = (size_t)tmp;

                fpos = ftell((FILE*)mat->fp);
                if ( fpos == -1L ) {
                    free(sparse->ir);
                    free(matvar->data);
                    matvar->data = NULL;
                    Mat_Critical("Couldn't determine file position");
                    return;
                }
                (void)fseek((FILE*)mat->fp,sparse->nir*Mat_SizeOf(data_type),
                    SEEK_CUR);
                ReadDoubleData(mat, &tmp, data_type, 1);
                if ( tmp > INT_MAX-1 || tmp < 0 ) {
                    free(sparse->ir);
                    free(matvar->data);
                    matvar->data = NULL;
                    Mat_Critical("Invalid column dimension for sparse matrix");
                    return;
                }
                matvar->dims[1] = (size_t)tmp;
                (void)fseek((FILE*)mat->fp,fpos,SEEK_SET);
                if ( matvar->dims[1] > INT_MAX-1 ) {
                    free(sparse->ir);
                    free(matvar->data);
                    matvar->data = NULL;
                    Mat_Critical("Invalid column dimension for sparse matrix");
                    return;
                }
                sparse->njc = (int)matvar->dims[1] + 1;
                sparse->jc = (mat_int32_t*)malloc(sparse->njc*sizeof(mat_int32_t));
                if ( sparse->jc != NULL ) {
                    mat_int32_t *jc;
                    jc = (mat_int32_t*)malloc(sparse->nir*sizeof(mat_int32_t));
                    if ( jc != NULL ) {
                        int j = 0;
                        sparse->jc[0] = 0;
                        ReadInt32Data(mat, jc, data_type, sparse->nir);
                        for ( i = 1; i < sparse->njc-1; i++ ) {
                            while ( j < sparse->nir && jc[j] <= i )
                                j++;
                            sparse->jc[i] = j;
                        }
                        free(jc);
                        /* terminating nnz */
                        sparse->jc[sparse->njc-1] = sparse->nir;
                    } else {
                        free(sparse->jc);
                        free(sparse->ir);
                        free(matvar->data);
                        matvar->data = NULL;
                        Mat_Critical("Couldn't allocate memory for the sparse index array");
                        return;
                    }
                } else {
                    free(sparse->ir);
                    free(matvar->data);
                    matvar->data = NULL;
                    Mat_Critical("Couldn't allocate memory for the sparse index array");
                    return;
                }
                ReadDoubleData(mat, &tmp, data_type, 1);
                sparse->ndata = sparse->nir;
                data_type = matvar->data_type;
                if ( matvar->isComplex ) {
                    mat_complex_split_t *complex_data =
                        ComplexMalloc(sparse->ndata*Mat_SizeOf(data_type));
                    if ( NULL != complex_data ) {
                        sparse->data = complex_data;
#if defined(EXTENDED_SPARSE)
                        switch ( data_type ) {
                            case MAT_T_DOUBLE:
                                ReadDoubleData(mat, (double*)complex_data->Re,
                                    data_type, sparse->ndata);
                                ReadDoubleData(mat, &tmp, data_type, 1);
                                ReadDoubleData(mat, (double*)complex_data->Im,
                                    data_type, sparse->ndata);
                                ReadDoubleData(mat, &tmp, data_type, 1);
                                break;
                            case MAT_T_SINGLE:
                            {
                                float tmp2;
                                ReadSingleData(mat, (float*)complex_data->Re,
                                    data_type, sparse->ndata);
                                ReadSingleData(mat, &tmp2, data_type, 1);
                                ReadSingleData(mat, (float*)complex_data->Im,
                                    data_type, sparse->ndata);
                                ReadSingleData(mat, &tmp2, data_type, 1);
                                break;
                            }
                            case MAT_T_INT32:
                            {
                                mat_int32_t tmp2;
                                ReadInt32Data(mat, (mat_int32_t*)complex_data->Re,
                                    data_type, sparse->ndata);
                                ReadInt32Data(mat, &tmp2, data_type, 1);
                                ReadInt32Data(mat, (mat_int32_t*)complex_data->Im,
                                    data_type, sparse->ndata);
                                ReadInt32Data(mat, &tmp2, data_type, 1);
                                break;
                            }
                            case MAT_T_INT16:
                            {
                                mat_int16_t tmp2;
                                ReadInt16Data(mat, (mat_int16_t*)complex_data->Re,
                                    data_type, sparse->ndata);
                                ReadInt16Data(mat, &tmp2, data_type, 1);
                                ReadInt16Data(mat, (mat_int16_t*)complex_data->Im,
                                    data_type, sparse->ndata);
                                ReadInt16Data(mat, &tmp2, data_type, 1);
                                break;
                            }
                            case MAT_T_UINT16:
                            {
                                mat_uint16_t tmp2;
                                ReadUInt16Data(mat, (mat_uint16_t*)complex_data->Re,
                                    data_type, sparse->ndata);
                                ReadUInt16Data(mat, &tmp2, data_type, 1);
                                ReadUInt16Data(mat, (mat_uint16_t*)complex_data->Im,
                                    data_type, sparse->ndata);
                                ReadUInt16Data(mat, &tmp2, data_type, 1);
                                break;
                            }
                            case MAT_T_UINT8:
                            {
                                mat_uint8_t tmp2;
                                ReadUInt8Data(mat, (mat_uint8_t*)complex_data->Re,
                                    data_type, sparse->ndata);
                                ReadUInt8Data(mat, &tmp2, data_type, 1);
                                ReadUInt8Data(mat, (mat_uint8_t*)complex_data->Im,
                                    data_type, sparse->ndata);
                                ReadUInt8Data(mat, &tmp2, data_type, 1);
                                break;
                            }
                            default:
                                free(complex_data->Re);
                                free(complex_data->Im);
                                free(complex_data);
                                free(sparse->jc);
                                free(sparse->ir);
                                free(matvar->data);
                                matvar->data = NULL;
                                Mat_Critical("Mat_VarRead4: %d is not a supported data type for "
                                    "extended sparse", data_type);
                                return;
                        }
#else
                        ReadDoubleData(mat, (double*)complex_data->Re,
                            data_type, sparse->ndata);
                        ReadDoubleData(mat, &tmp, data_type, 1);
                        ReadDoubleData(mat, (double*)complex_data->Im,
                            data_type, sparse->ndata);
                        ReadDoubleData(mat, &tmp, data_type, 1);
#endif
                    }
                    else {
                        free(sparse->jc);
                        free(sparse->ir);
                        free(matvar->data);
                        matvar->data = NULL;
                        Mat_Critical("Couldn't allocate memory for the complex sparse data");
                        return;
                    }
                } else {
                    sparse->data = malloc(sparse->ndata*Mat_SizeOf(data_type));
                    if ( sparse->data != NULL ) {
#if defined(EXTENDED_SPARSE)
                        switch ( data_type ) {
                            case MAT_T_DOUBLE:
                                ReadDoubleData(mat, (double*)sparse->data,
                                    data_type, sparse->ndata);
                                ReadDoubleData(mat, &tmp, data_type, 1);
                                break;
                            case MAT_T_SINGLE:
                            {
                                float tmp2;
                                ReadSingleData(mat, (float*)sparse->data,
                                    data_type, sparse->ndata);
                                ReadSingleData(mat, &tmp2, data_type, 1);
                                break;
                            }
                            case MAT_T_INT32:
                            {
                                mat_int32_t tmp2;
                                ReadInt32Data(mat, (mat_int32_t*)sparse->data,
                                    data_type, sparse->ndata);
                                ReadInt32Data(mat, &tmp2, data_type, 1);
                                break;
                            }
                            case MAT_T_INT16:
                            {
                                mat_int16_t tmp2;
                                ReadInt16Data(mat, (mat_int16_t*)sparse->data,
                                    data_type, sparse->ndata);
                                ReadInt16Data(mat, &tmp2, data_type, 1);
                                break;
                            }
                            case MAT_T_UINT16:
                            {
                                mat_uint16_t tmp2;
                                ReadUInt16Data(mat, (mat_uint16_t*)sparse->data,
                                    data_type, sparse->ndata);
                                ReadUInt16Data(mat, &tmp2, data_type, 1);
                                break;
                            }
                            case MAT_T_UINT8:
                            {
                                mat_uint8_t tmp2;
                                ReadUInt8Data(mat, (mat_uint8_t*)sparse->data,
                                    data_type, sparse->ndata);
                                ReadUInt8Data(mat, &tmp2, data_type, 1);
                                break;
                            }
                            default:
                                free(sparse->data);
                                free(sparse->jc);
                                free(sparse->ir);
                                free(matvar->data);
                                matvar->data = NULL;
                                Mat_Critical("Mat_VarRead4: %d is not a supported data type for "
                                    "extended sparse", data_type);
                                return;
                        }
#else
                        ReadDoubleData(mat, (double*)sparse->data, data_type, sparse->ndata);
                        ReadDoubleData(mat, &tmp, data_type, 1);
#endif
                    } else {
                        free(sparse->jc);
                        free(sparse->ir);
                        free(matvar->data);
                        matvar->data = NULL;
                        Mat_Critical("Couldn't allocate memory for the sparse data");
                        return;
                    }
                }
                break;
            }
            else {
                Mat_Critical("Couldn't allocate memory for the data");
                return;
            }
        default:
            Mat_Critical("MAT V4 data type error");
            return;
    }

    return;
}
