hfs_file_read_zlib_rsrc(const TSK_FS_ATTR * a_fs_attr,
    TSK_OFF_T a_offset, char *a_buf, size_t a_len)
{
    return hfs_file_read_compressed_rsrc(
        a_fs_attr, a_offset, a_buf, a_len,
        hfs_read_zlib_block_table,
        hfs_decompress_zlib_block
    );
}
