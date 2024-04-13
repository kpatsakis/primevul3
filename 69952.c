Mat_VarReadDataLinear4(mat_t *mat,matvar_t *matvar,void *data,int start,
                       int stride,int edge)
{
    int err;
    size_t nelems = 1;

    err = SafeMulDims(matvar, &nelems);
    if ( err ) {
        Mat_Critical("Integer multiplication overflow");
        return err;
    }

    (void)fseek((FILE*)mat->fp,matvar->internal->datapos,SEEK_SET);

    matvar->data_size = Mat_SizeOf(matvar->data_type);

    if ( (size_t)stride*(edge-1)+start+1 > nelems ) {
        return 1;
    }
    if ( matvar->isComplex ) {
        mat_complex_split_t *complex_data = (mat_complex_split_t*)data;
        err = SafeMul(&nelems, nelems, matvar->data_size);
        if ( err ) {
            Mat_Critical("Integer multiplication overflow");
            return err;
        }

        ReadDataSlab1(mat,complex_data->Re,matvar->class_type,
                      matvar->data_type,start,stride,edge);
        (void)fseek((FILE*)mat->fp,matvar->internal->datapos+nelems,SEEK_SET);
        ReadDataSlab1(mat,complex_data->Im,matvar->class_type,
                      matvar->data_type,start,stride,edge);
    } else {
        ReadDataSlab1(mat,data,matvar->class_type,matvar->data_type,start,
                      stride,edge);
    }

    return err;
}
