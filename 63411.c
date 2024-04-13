static int hfs_file_read_zlib_attr(TSK_FS_FILE* fs_file,
                            char* buffer,
                            uint32_t attributeLength,
                            uint64_t uncSize)
{
    return hfs_file_read_compressed_attr(
        fs_file, DECMPFS_TYPE_ZLIB_ATTR,
        buffer, attributeLength, uncSize,
        hfs_decompress_zlib_attr
    );
}
