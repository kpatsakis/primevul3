static int hfs_decompress_lzvn_block(char* rawBuf, uint32_t len, char* uncBuf, uint64_t* uncLen)
{
    if (len > 0 && rawBuf[0] != 0x06) {
        *uncLen = lzvn_decode_buffer(uncBuf, COMPRESSION_UNIT_SIZE, rawBuf, len);
        return 1;  // apparently this can't fail
    }
    else {
        return hfs_decompress_noncompressed_block(rawBuf, len, uncBuf, uncLen);
    }
}
