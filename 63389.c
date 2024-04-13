hfs_block_getflags(TSK_FS_INFO * a_fs, TSK_DADDR_T a_addr)
{
    return (hfs_block_is_alloc((HFS_INFO *) a_fs, a_addr) == 1) ?
        TSK_FS_BLOCK_FLAG_ALLOC : TSK_FS_BLOCK_FLAG_UNALLOC;
}
