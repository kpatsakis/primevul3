hfs_inode_lookup(TSK_FS_INFO * fs, TSK_FS_FILE * a_fs_file,
    TSK_INUM_T inum)
{
    HFS_INFO *hfs = (HFS_INFO *) fs;
    HFS_ENTRY entry;

    if (a_fs_file == NULL) {
        tsk_error_set_errno(TSK_ERR_FS_ARG);
        tsk_error_set_errstr("hfs_inode_lookup: fs_file is NULL");
        return 1;
    }

    if (a_fs_file->meta == NULL) {
        a_fs_file->meta = tsk_fs_meta_alloc(HFS_FILE_CONTENT_LEN);
    }

    if (a_fs_file->meta == NULL) {
        return 1;
    }
    else {
        tsk_fs_meta_reset(a_fs_file->meta);
    }

    if (tsk_verbose)
        tsk_fprintf(stderr, "hfs_inode_lookup: looking up %" PRIuINUM "\n",
            inum);


    /* First see if this is a special entry
     * the special ones have their metadata stored in the volume header */
    if (inum == HFS_EXTENTS_FILE_ID) {
        if (!hfs->has_extents_file) {
            error_detected(TSK_ERR_FS_INODE_NUM,
                "Extents File not present");
            return 1;
        }

        return hfs_make_extents(hfs, a_fs_file);
    }
    else if (inum == HFS_CATALOG_FILE_ID) {
        return hfs_make_catalog(hfs, a_fs_file);
    }
    else if (inum == HFS_BAD_BLOCK_FILE_ID) {
        if (!hfs->has_extents_file) {
            error_detected(TSK_ERR_FS_INODE_NUM,
                "BadBlocks File not present");
            return 1;
        }
        return hfs_make_badblockfile(hfs, a_fs_file);
    }
    else if (inum == HFS_ALLOCATION_FILE_ID) {
        return hfs_make_blockmap(hfs, a_fs_file);
    }
    else if (inum == HFS_STARTUP_FILE_ID) {
        if (!hfs->has_startup_file) {
            error_detected(TSK_ERR_FS_INODE_NUM,
                "Startup File not present");
            return 1;
        }
        return hfs_make_startfile(hfs, a_fs_file);
    }
    else if (inum == HFS_ATTRIBUTES_FILE_ID) {
        if (!hfs->has_attributes_file) {
            error_detected(TSK_ERR_FS_INODE_NUM,
                "Attributes File not present");
            return 1;
        }
        return hfs_make_attrfile(hfs, a_fs_file);
    }

    /* Lookup inode and store it in the HFS structure */
    if (hfs_cat_file_lookup(hfs, inum, &entry, TRUE)) {
        return 1;
    }

    /* Copy the structure in hfs to generic fs_inode */
    if (hfs_dinode_copy(hfs, &entry, a_fs_file)) {
        return 1;
    }

    /* If this is potentially a compressed file, its
     * actual size is unknown until we examine the
     * extended attributes */
    if ((a_fs_file->meta->size == 0) &&
        (a_fs_file->meta->type == TSK_FS_META_TYPE_REG) &&
        (a_fs_file->meta->attr_state != TSK_FS_META_ATTR_ERROR) &&
        ((a_fs_file->meta->attr_state != TSK_FS_META_ATTR_STUDIED) ||
            (a_fs_file->meta->attr == NULL))) {
        hfs_load_attrs(a_fs_file);
    }

    return 0;
}
