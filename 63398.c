hfs_close(TSK_FS_INFO * fs)
{
    HFS_INFO *hfs = (HFS_INFO *) fs;
    tsk_take_lock(&(hfs->metadata_dir_cache_lock));
    fs->tag = 0;

    free(hfs->fs);

    if (hfs->catalog_file) {
        tsk_fs_file_close(hfs->catalog_file);
        hfs->catalog_attr = NULL;
    }

    if (hfs->blockmap_file) {
        tsk_fs_file_close(hfs->blockmap_file);
        hfs->blockmap_attr = NULL;
    }

    if (hfs->meta_dir) {
        tsk_fs_dir_close(hfs->meta_dir);
        hfs->meta_dir = NULL;
    }

    if (hfs->dir_meta_dir) {
        tsk_fs_dir_close(hfs->dir_meta_dir);
        hfs->dir_meta_dir = NULL;
    }

    if (hfs->extents_file) {
        tsk_fs_file_close(hfs->extents_file);
        hfs->extents_file = NULL;
    }

    tsk_release_lock(&(hfs->metadata_dir_cache_lock));
    tsk_deinit_lock(&(hfs->metadata_dir_cache_lock));

    tsk_fs_free((TSK_FS_INFO *)hfs);
}
