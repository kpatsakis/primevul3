static int hfs_decompress_zlib_attr(char* rawBuf, uint32_t rawSize, uint64_t uncSize, char** dstBuf, uint64_t* dstSize, int* dstBufFree)
{
    if ((rawBuf[0] & 0x0F) == 0x0F) {
        return hfs_decompress_noncompressed_attr(
            rawBuf, rawSize, uncSize, dstBuf, dstSize, dstBufFree);
    }
    else {
#ifdef HAVE_LIBZ
        char* uncBuf = NULL;
        uint64_t uLen;
        unsigned long bytesConsumed;
        int infResult;

        if (tsk_verbose)
            tsk_fprintf(stderr,
                        "%s: Uncompressing (inflating) data.", __func__);

        uncBuf = (char *) tsk_malloc((size_t) uncSize + 100); // add some extra space
        if (uncBuf == NULL) {
            error_returned
                (" - %s, space for the uncompressed attr", __func__);
            return 0;
        }

        infResult = zlib_inflate(rawBuf, (uint64_t) rawSize,
                                 uncBuf, (uint64_t) (uncSize + 100),
                                 &uLen, &bytesConsumed);
        if (infResult != 0) {
            error_returned
                (" %s, zlib could not uncompress attr", __func__);
            free(uncBuf);
            return 0;
        }

        if (bytesConsumed != rawSize) {
            error_detected(TSK_ERR_FS_READ,
                " %s, decompressor did not consume the whole compressed data",
                __func__);
            free(uncBuf);
            return 0;
        }

        *dstBuf = uncBuf;
        *dstSize = uncSize;
        *dstBufFree = TRUE;
#else

        if (tsk_verbose)
            tsk_fprintf(stderr,
                        "%s: ZLIB not available, so loading an empty default DATA attribute.\n", __func__);

        static uint8_t dummy[1];

        *dstBuf = dummy;
        *dstSize = 0;
        *dstBufFree = FALSE;
#endif
    }

    return 1;
}
