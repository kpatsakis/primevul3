hfs_block_is_alloc(HFS_INFO * hfs, TSK_DADDR_T a_addr)
{
    TSK_FS_INFO *fs = &(hfs->fs_info);
    TSK_OFF_T b;
    size_t b2;

    if (hfs->blockmap_file == NULL) {
        if ((hfs->blockmap_file =
                tsk_fs_file_open_meta(fs, NULL,
                    HFS_ALLOCATION_FILE_ID)) == NULL) {
            tsk_error_errstr2_concat(" - Loading blockmap file");
            return -1;
        }

        /* cache the data attribute */
        hfs->blockmap_attr =
            tsk_fs_attrlist_get(hfs->blockmap_file->meta->attr,
            TSK_FS_ATTR_TYPE_DEFAULT);
        if (!hfs->blockmap_attr) {
            tsk_error_errstr2_concat
                (" - Data Attribute not found in Blockmap File");
            return -1;
        }
        hfs->blockmap_cache_start = -1;
        hfs->blockmap_cache_len = 0;
    }

    b = (TSK_OFF_T) a_addr / 8;
    if (b > hfs->blockmap_file->meta->size) {
        tsk_error_set_errno(TSK_ERR_FS_CORRUPT);
        tsk_error_set_errstr("hfs_block_is_alloc: block %" PRIuDADDR
            " is too large for bitmap (%" PRIuOFF ")", a_addr,
            hfs->blockmap_file->meta->size);
        return -1;
    }

    if ((hfs->blockmap_cache_start == -1)
        || (hfs->blockmap_cache_start > b)
        || (hfs->blockmap_cache_start + hfs->blockmap_cache_len <= b)) {
        size_t cnt = tsk_fs_attr_read(hfs->blockmap_attr, b,
            hfs->blockmap_cache,
            sizeof(hfs->blockmap_cache), 0);
        if (cnt < 1) {
            tsk_error_set_errstr2
                ("hfs_block_is_alloc: Error reading block bitmap at offset %"
                PRIuOFF, b);
            return -1;
        }
        hfs->blockmap_cache_start = b;
        hfs->blockmap_cache_len = cnt;
    }
    b2 = (size_t) (b - hfs->blockmap_cache_start);
    return (hfs->blockmap_cache[b2] & (1 << (7 - (a_addr % 8)))) != 0;
}
