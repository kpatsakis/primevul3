hfs_checked_read_random(TSK_FS_INFO * fs, char *buf, size_t len,
    TSK_OFF_T offs)
{
    ssize_t r;

    r = tsk_fs_read(fs, offs, buf, len);
    if (r != len) {
        if (r >= 0) {
            tsk_error_reset();
            tsk_error_set_errno(TSK_ERR_FS_READ);
        }
        return 1;
    }
    return 0;
}
