hfs_attr_walk_zlib_rsrc(const TSK_FS_ATTR * fs_attr,
    int flags, TSK_FS_FILE_WALK_CB a_action, void *ptr)
{
    return hfs_attr_walk_compressed_rsrc(
      fs_attr, flags, a_action, ptr,
      hfs_read_zlib_block_table,
      hfs_decompress_zlib_block
    );
}
