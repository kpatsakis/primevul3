hfs_cat_read_thread_record(HFS_INFO * hfs, TSK_OFF_T off,
    hfs_thread * thread)
{
    TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);
    uint16_t uni_len;
    ssize_t cnt;

    memset(thread, 0, sizeof(hfs_thread));
    cnt = tsk_fs_attr_read(hfs->catalog_attr, off, (char *) thread, 10, 0);
    if (cnt != 10) {
        if (cnt >= 0) {
            tsk_error_reset();
            tsk_error_set_errno(TSK_ERR_FS_READ);
        }
        tsk_error_set_errstr2
            ("hfs_cat_read_thread_record: Error reading catalog offset %"
            PRIuOFF " (header)", off);
        return 1;
    }

    if ((tsk_getu16(fs->endian, thread->rec_type) != HFS_FOLDER_THREAD)
        && (tsk_getu16(fs->endian, thread->rec_type) != HFS_FILE_THREAD)) {
        tsk_error_set_errno(TSK_ERR_FS_GENFS);
        tsk_error_set_errstr
            ("hfs_cat_read_thread_record: unexpected record type %" PRIu16,
            tsk_getu16(fs->endian, thread->rec_type));
        return 1;
    }

    uni_len = tsk_getu16(fs->endian, thread->name.length);

    if (uni_len > 255) {
        tsk_error_set_errno(TSK_ERR_FS_INODE_COR);
        tsk_error_set_errstr
            ("hfs_cat_read_thread_record: invalid string length (%" PRIu16
            ")", uni_len);
        return 1;
    }

    cnt =
        tsk_fs_attr_read(hfs->catalog_attr, off + 10,
        (char *) thread->name.unicode, uni_len * 2, 0);
    if (cnt != uni_len * 2) {
        if (cnt >= 0) {
            tsk_error_reset();
            tsk_error_set_errno(TSK_ERR_FS_READ);
        }
        tsk_error_set_errstr2
            ("hfs_cat_read_thread_record: Error reading catalog offset %"
            PRIuOFF " (name)", off + 10);
        return 1;
    }

    return 0;
}
