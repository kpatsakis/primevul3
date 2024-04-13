static int hfs_file_read_lzvn_attr(TSK_FS_FILE* fs_file,
                            char* buffer,
                            uint32_t attributeLength,
                            uint64_t uncSize)
{
    return hfs_file_read_compressed_attr(
        fs_file, DECMPFS_TYPE_LZVN_ATTR,
        buffer, attributeLength, uncSize,
        hfs_decompress_lzvn_attr
    );
}
