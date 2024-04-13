hfs_cat_read_file_folder_record(HFS_INFO * hfs, TSK_OFF_T off,
    hfs_file_folder * record)
{
    TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);
    ssize_t cnt;
    char rec_type[2];

    memset(record, 0, sizeof(hfs_file_folder));

    cnt = tsk_fs_attr_read(hfs->catalog_attr, off, rec_type, 2, 0);
    if (cnt != 2) {
        if (cnt >= 0) {
            tsk_error_reset();
            tsk_error_set_errno(TSK_ERR_FS_READ);
        }
        tsk_error_set_errstr2
            ("hfs_cat_read_file_folder_record: Error reading record type from catalog offset %"
            PRIuOFF " (header)", off);
        return 1;
    }

    if (tsk_getu16(fs->endian, rec_type) == HFS_FOLDER_RECORD) {
        cnt =
            tsk_fs_attr_read(hfs->catalog_attr, off, (char *) record,
            sizeof(hfs_folder), 0);
        if (cnt != sizeof(hfs_folder)) {
            if (cnt >= 0) {
                tsk_error_reset();
                tsk_error_set_errno(TSK_ERR_FS_READ);
            }
            tsk_error_set_errstr2
                ("hfs_cat_read_file_folder_record: Error reading catalog offset %"
                PRIuOFF " (folder)", off);
            return 1;
        }
    }
    else if (tsk_getu16(fs->endian, rec_type) == HFS_FILE_RECORD) {
        cnt =
            tsk_fs_attr_read(hfs->catalog_attr, off, (char *) record,
            sizeof(hfs_file), 0);
        if (cnt != sizeof(hfs_file)) {
            if (cnt >= 0) {
                tsk_error_reset();
                tsk_error_set_errno(TSK_ERR_FS_READ);
            }
            tsk_error_set_errstr2
                ("hfs_cat_read_file_folder_record: Error reading catalog offset %"
                PRIuOFF " (file)", off);
            return 1;
        }
    }
    else {
        tsk_error_set_errno(TSK_ERR_FS_GENFS);
        tsk_error_set_errstr
            ("hfs_cat_read_file_folder_record: unexpected record type %"
            PRIu16, tsk_getu16(fs->endian, rec_type));
        return 1;
    }

    return 0;
}
