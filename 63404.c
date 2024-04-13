hfs_dinode_copy(HFS_INFO * a_hfs, const HFS_ENTRY * a_hfs_entry,
    TSK_FS_FILE * a_fs_file)
{

    const hfs_file_folder *a_entry =
        (hfs_file_folder *) & (a_hfs_entry->cat);
    const hfs_file_fold_std *std;
    TSK_FS_META *a_fs_meta = a_fs_file->meta;
    TSK_FS_INFO *fs;
    uint16_t hfsmode;
    TSK_INUM_T iStd;            // the inum (or CNID) that occurs in the standard file metadata

    if (a_entry == NULL) {
        error_detected(TSK_ERR_FS_ARG,
            "hfs_dinode_copy: a_entry = a_hfs_entry->cat is NULL");
        return 1;
    }

    fs = (TSK_FS_INFO *) & a_hfs->fs_info;


    iStd = tsk_getu32(fs->endian, a_entry->file.std.cnid);
    if (iStd != a_hfs_entry->inum) {
        if (tsk_verbose)
            tsk_fprintf(stderr,
                "WARNING: hfs_dinode_copy:  HFS_ENTRY with conflicting values for inum (or cnid).\n");
    }

    if (a_fs_meta == NULL) {
        tsk_error_set_errno(TSK_ERR_FS_ARG);
        tsk_error_set_errstr("hfs_dinode_copy: a_fs_meta is NULL");
        return 1;
    }

    std = &(a_entry->file.std);

    if (tsk_verbose)
        tsk_fprintf(stderr,
            "hfs_dinode_copy: called for file/folder %" PRIu32 "\n",
            tsk_getu32(fs->endian, std->cnid));

    if (a_fs_meta->content_len < HFS_FILE_CONTENT_LEN) {
        if ((a_fs_meta =
                tsk_fs_meta_realloc(a_fs_meta,
                    HFS_FILE_CONTENT_LEN)) == NULL) {
            return 1;
        }
    }
    a_fs_meta->attr_state = TSK_FS_META_ATTR_EMPTY;
    if (a_fs_meta->attr) {
        tsk_fs_attrlist_markunused(a_fs_meta->attr);
    }


    /*
     * Copy the file type specific stuff first
     */
    hfsmode = tsk_getu16(fs->endian, std->perm.mode);

    if (tsk_getu16(fs->endian, std->rec_type) == HFS_FOLDER_RECORD) {
        if ((hfsmode & HFS_IN_IFMT) == 0)
            a_fs_meta->type = TSK_FS_META_TYPE_DIR;
        a_fs_meta->size = 0;
        memset(a_fs_meta->content_ptr, 0, HFS_FILE_CONTENT_LEN);
    }
    else if (tsk_getu16(fs->endian, std->rec_type) == HFS_FILE_RECORD) {
        hfs_fork *fork;
        if ((hfsmode & HFS_IN_IFMT) == 0)
            a_fs_meta->type = TSK_FS_META_TYPE_REG;
        a_fs_meta->size =
            tsk_getu64(fs->endian, a_entry->file.data.logic_sz);

        fork = (hfs_fork *) a_fs_meta->content_ptr;
        memcpy(fork, &(a_entry->file.data), sizeof(hfs_fork));
        memcpy(&fork[1], &(a_entry->file.resource), sizeof(hfs_fork));
    }
    else {
        if (tsk_verbose)
            tsk_fprintf(stderr,
                "hfs_dinode_copy error: catalog entry is neither file nor folder\n");
        return 1;
    }

    /*
     * Copy the standard stuff.
     * Use default values (as defined in spec) if mode is not defined.
     */
    if ((hfsmode & HFS_IN_IFMT) == 0) {
        a_fs_meta->mode = 0;
        a_fs_meta->uid = 99;
        a_fs_meta->gid = 99;
    }
    else {
        a_fs_meta->mode = hfs_mode_to_tsk_mode(hfsmode);
        a_fs_meta->type = hfs_mode_to_tsk_meta_type(hfsmode);
        a_fs_meta->uid = tsk_getu32(fs->endian, std->perm.owner);
        a_fs_meta->gid = tsk_getu32(fs->endian, std->perm.group);
    }

    if (tsk_getu32(fs->endian, std->perm.special.nlink))
        a_fs_meta->nlink = tsk_getu32(fs->endian, std->perm.special.nlink);
    else
        a_fs_meta->nlink = 1;

    a_fs_meta->mtime =
        hfs_convert_2_unix_time(tsk_getu32(fs->endian, std->cmtime));
    a_fs_meta->atime =
        hfs_convert_2_unix_time(tsk_getu32(fs->endian, std->atime));
    a_fs_meta->crtime =
        hfs_convert_2_unix_time(tsk_getu32(fs->endian, std->crtime));
    a_fs_meta->ctime =
        hfs_convert_2_unix_time(tsk_getu32(fs->endian, std->amtime));
    a_fs_meta->time2.hfs.bkup_time =
        hfs_convert_2_unix_time(tsk_getu32(fs->endian, std->bkup_date));
    a_fs_meta->mtime_nano = a_fs_meta->atime_nano = a_fs_meta->ctime_nano =
        a_fs_meta->crtime_nano = 0;
    a_fs_meta->time2.hfs.bkup_time_nano = 0;

    a_fs_meta->addr = tsk_getu32(fs->endian, std->cnid);

    a_fs_meta->flags = TSK_FS_META_FLAG_ALLOC | TSK_FS_META_FLAG_USED;

    if (std->perm.o_flags & HFS_PERM_OFLAG_COMPRESSED)
        a_fs_meta->flags |= TSK_FS_META_FLAG_COMP;


    if (a_fs_file->name != NULL) {
        a_fs_file->name->meta_addr = a_fs_meta->addr;
    }

    /* TODO @@@ could fill in name2 with this entry's name and parent inode
       from Catalog entry */

    /* set the link string (if the file is a link)
     * The size check is a sanity check so that we don't try to allocate
     * a huge amount of memory for a bad inode value
     */
    if ((a_fs_meta->type == TSK_FS_META_TYPE_LNK) &&
        (a_fs_meta->size >= 0) && (a_fs_meta->size < HFS_MAXPATHLEN)) {

        ssize_t bytes_read;

        a_fs_meta->link = tsk_malloc((size_t) a_fs_meta->size + 1);
        if (a_fs_meta->link == NULL)
            return 1;

        bytes_read = tsk_fs_file_read(a_fs_file, (TSK_OFF_T) 0,
            a_fs_meta->link, (size_t) a_fs_meta->size,
            TSK_FS_FILE_READ_FLAG_NONE);
        a_fs_meta->link[a_fs_meta->size] = '\0';

        if (bytes_read != a_fs_meta->size) {
            if (tsk_verbose)
                tsk_fprintf(stderr,
                    "hfs_dinode_copy: failed to read contents of symbolic link; "
                    "expected %u bytes but tsk_fs_file_read() returned %u\n",
                    a_fs_meta->size, bytes_read);
            free(a_fs_meta->link);
            a_fs_meta->link = NULL;
            return 1;
        }
    }

    return 0;
}
