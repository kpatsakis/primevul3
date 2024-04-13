static int hfs_decompress_zlib_block(char* rawBuf, uint32_t len, char* uncBuf, uint64_t* uncLen)
{
    if (len > 0 && (rawBuf[0] & 0x0F) != 0x0F) {
        if (tsk_verbose)
            tsk_fprintf(stderr,
                        "%s: Inflating the compression unit\n", __func__);

        unsigned long bytesConsumed;
        int infResult = zlib_inflate(rawBuf, (uint64_t) len,
            uncBuf, (uint64_t) COMPRESSION_UNIT_SIZE,
            uncLen, &bytesConsumed);
        if (infResult != 0) {
            error_returned
                  (" %s: zlib inflation (uncompression) failed",
                  __func__, infResult);
            return 0;
        }

        if (bytesConsumed != len) {
            error_detected(TSK_ERR_FS_READ,
                " %s, decompressor did not consume the whole compressed data",
                __func__);
            return 0;
        }

        return 1;
    }
    else {
        return hfs_decompress_noncompressed_block(rawBuf, len, uncBuf, uncLen);
    }
}
