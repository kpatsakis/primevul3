hfs_cat_file_lookup(HFS_INFO * hfs, TSK_INUM_T inum, HFS_ENTRY * entry,
    unsigned char follow_hard_link)
{
    TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);
    hfs_btree_key_cat key;      /* current catalog key */
    hfs_thread thread;          /* thread record */
    hfs_file_folder record;     /* file/folder record */
    TSK_OFF_T off;

    tsk_error_reset();

    if (tsk_verbose)
        tsk_fprintf(stderr,
            "hfs_cat_file_lookup: called for inum %" PRIuINUM "\n", inum);

    if ((inum == HFS_EXTENTS_FILE_ID) ||
        (inum == HFS_CATALOG_FILE_ID) ||
        (inum == HFS_ALLOCATION_FILE_ID) ||
        (inum == HFS_STARTUP_FILE_ID) ||
        (inum == HFS_ATTRIBUTES_FILE_ID)) {
        tsk_error_set_errno(TSK_ERR_FS_GENFS);
        tsk_error_set_errstr
            ("hfs_cat_file_lookup: Called on special file: %" PRIuINUM,
            inum);
        return 1;
    }


    /* first look up the thread record for the item we're searching for */

    /* set up the thread record key */
    memset((char *) &key, 0, sizeof(hfs_btree_key_cat));
    cnid_to_array((uint32_t) inum, key.parent_cnid);

    if (tsk_verbose)
        tsk_fprintf(stderr,
            "hfs_cat_file_lookup: Looking up thread record (%" PRIuINUM
            ")\n", inum);

    /* look up the thread record */
    off = hfs_cat_get_record_offset(hfs, &key);
    if (off == 0) {
        if (tsk_error_get_errno() == 0) {
            tsk_error_set_errno(TSK_ERR_FS_INODE_NUM);
            tsk_error_set_errstr
                ("hfs_cat_file_lookup: Error finding thread node for file (%"
                PRIuINUM ")", inum);
        }
        else {
            tsk_error_set_errstr2
                (" hfs_cat_file_lookup: thread for file (%" PRIuINUM ")",
                inum);
        }
        return 1;
    }

    /* read the thread record */
    if (hfs_cat_read_thread_record(hfs, off, &thread)) {
        tsk_error_set_errstr2(" hfs_cat_file_lookup: file (%" PRIuINUM ")",
            inum);
        return 1;
    }

    /* now look up the actual file/folder record */

    /* build key */
    memset((char *) &key, 0, sizeof(hfs_btree_key_cat));
    memcpy((char *) key.parent_cnid, (char *) thread.parent_cnid,
        sizeof(key.parent_cnid));
    memcpy((char *) &key.name, (char *) &thread.name, sizeof(key.name));

    if (tsk_verbose)
        tsk_fprintf(stderr,
            "hfs_cat_file_lookup: Looking up file record (parent: %"
            PRIuINUM ")\n", (uint64_t) tsk_getu32(fs->endian,
                key.parent_cnid));

    /* look up the record */
    off = hfs_cat_get_record_offset(hfs, &key);
    if (off == 0) {
        if (tsk_error_get_errno() == 0) {
            tsk_error_set_errno(TSK_ERR_FS_INODE_NUM);
            tsk_error_set_errstr
                ("hfs_cat_file_lookup: Error finding record node %"
                PRIuINUM, inum);
        }
        else {
            tsk_error_set_errstr2(" hfs_cat_file_lookup: file (%" PRIuINUM
                ")", inum);
        }
        return 1;
    }

    /* read the record */
    if (hfs_cat_read_file_folder_record(hfs, off, &record)) {
        tsk_error_set_errstr2(" hfs_cat_file_lookup: file (%" PRIuINUM ")",
            inum);
        return 1;
    }

    /* these memcpy can be gotten rid of, really */
    if (tsk_getu16(fs->endian,
            record.file.std.rec_type) == HFS_FOLDER_RECORD) {
        if (tsk_verbose)
            tsk_fprintf(stderr,
                "hfs_cat_file_lookup: found folder record valence %" PRIu32
                ", cnid %" PRIu32 "\n", tsk_getu32(fs->endian,
                    record.folder.std.valence), tsk_getu32(fs->endian,
                    record.folder.std.cnid));
        memcpy((char *) &entry->cat, (char *) &record, sizeof(hfs_folder));
    }
    else if (tsk_getu16(fs->endian,
            record.file.std.rec_type) == HFS_FILE_RECORD) {
        if (tsk_verbose)
            tsk_fprintf(stderr,
                "hfs_cat_file_lookup: found file record cnid %" PRIu32
                "\n", tsk_getu32(fs->endian, record.file.std.cnid));
        memcpy((char *) &entry->cat, (char *) &record, sizeof(hfs_file));
    }
    /* other cases already caught by hfs_cat_read_file_folder_record */

    memcpy((char *) &entry->thread, (char *) &thread, sizeof(hfs_thread));

    entry->flags = TSK_FS_META_FLAG_ALLOC | TSK_FS_META_FLAG_USED;
    entry->inum = inum;

    if (follow_hard_link) {
        unsigned char is_err;
        TSK_INUM_T target_cnid =
            hfs_follow_hard_link(hfs, &(entry->cat), &is_err);
        if (is_err > 1) {
            error_returned
                ("hfs_cat_file_lookup: error occurred while following a possible hard link for "
                "inum (cnid) =  %" PRIuINUM, inum);
            return 1;
        }
        if (target_cnid != inum) {
            uint8_t res =
                hfs_cat_file_lookup(hfs, target_cnid, entry, FALSE);
            if (res != 0) {
                error_returned
                    ("hfs_cat_file_lookup: error occurred while looking up the Catalog entry for "
                    "the target of inum (cnid) = %" PRIuINUM " target",
                    inum);
            }
            return 1;
        }

    }

    if (tsk_verbose)
        tsk_fprintf(stderr, "hfs_cat_file_lookup exiting\n");
    return 0;
}
