hfs_file_read_compressed_attr(TSK_FS_FILE* fs_file,
                              uint8_t cmpType,
                              char* buffer,
                              uint32_t attributeLength,
                              uint64_t uncSize,
                              int (*decompress_attr)(char* rawBuf,
                                                     uint32_t rawSize,
                                                     uint64_t uncSize,
                                                     char** dstBuf,
                                                     uint64_t* dstSize,
                                                     int* dstBufFree))
{
    if (tsk_verbose)
        tsk_fprintf(stderr,
            "%s: Compressed data is inline in the attribute, will load this as the default DATA attribute.\n", __func__);

    if (attributeLength <= 16) {
        if (tsk_verbose)
            tsk_fprintf(stderr,
                "%s: WARNING, Compression Record of type %u is not followed by"
                " compressed data. No data will be loaded into the DATA"
                " attribute.\n", __func__, cmpType);

        return 1;
    }

    TSK_FS_ATTR *fs_attr_unc;

    if ((fs_attr_unc = tsk_fs_attrlist_getnew(
          fs_file->meta->attr, TSK_FS_ATTR_RES)) == NULL)
    {
        error_returned(" - %s, FS_ATTR for uncompressed data", __func__);
        return 0;
    }

    char* dstBuf;
    uint64_t dstSize;
    int dstBufFree = FALSE;

    if (!decompress_attr(buffer + 16, attributeLength - 16, uncSize,
                         &dstBuf, &dstSize, &dstBufFree)) {
        return 0;
    }

    if (dstSize != uncSize) {
        error_detected(TSK_ERR_FS_READ,
            " %s, actual uncompressed size not equal to the size in the compression record", __func__);
        goto on_error;
    }

    if (tsk_verbose)
       tsk_fprintf(stderr,
                   "%s: Loading decompressed data as default DATA attribute.",
                   __func__);

    if (tsk_fs_attr_set_str(fs_file,
                            fs_attr_unc, "DATA",
                            TSK_FS_ATTR_TYPE_HFS_DATA,
                            HFS_FS_ATTR_ID_DATA, dstBuf,
                            dstSize))
    {
        error_returned(" - %s", __func__);
        goto on_error;
    }

    if (dstBufFree) {
        free(dstBuf);
    }
    return 1;

on_error:
    if (dstBufFree) {
        free(dstBuf);
    }
    return 0;
}
