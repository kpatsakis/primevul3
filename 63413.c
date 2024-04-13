hfs_follow_hard_link(HFS_INFO * hfs, hfs_file * cat,
    unsigned char *is_error)
{

    TSK_FS_INFO *fs = (TSK_FS_INFO *) hfs;
    TSK_INUM_T cnid;
    time_t crtime;
    uint32_t file_type;
    uint32_t file_creator;

    *is_error = 0;              // default, not an error

    if (cat == NULL) {
        error_detected(TSK_ERR_FS_ARG,
            "hfs_follow_hard_link: Pointer to Catalog entry (2nd arg) is null");
        return 0;
    }

    cnid = tsk_getu32(fs->endian, cat->std.cnid);

    if (cnid < HFS_FIRST_USER_CNID) {
        return cnid;
    }

    crtime =
        (time_t) hfs_convert_2_unix_time(tsk_getu32(fs->endian,
            cat->std.crtime));


    file_type = tsk_getu32(fs->endian, cat->std.u_info.file_type);
    file_creator = tsk_getu32(fs->endian, cat->std.u_info.file_cr);

    if (file_type == HFS_HARDLINK_FILE_TYPE
        && file_creator == HFS_HARDLINK_FILE_CREATOR) {

        if (hfs->meta_inum == 0)
            return cnid;

        if ((!hfs->has_root_crtime) && (!hfs->has_meta_dir_crtime)
            && (!hfs->has_meta_crtime)) {
            uint32_t linkNum =
                tsk_getu32(fs->endian, cat->std.perm.special.inum);
            *is_error = 1;
            if (tsk_verbose)
                tsk_fprintf(stderr,
                    "WARNING: hfs_follow_hard_link: File system creation times are not set. "
                    "Cannot test inode for hard link. File type and creator indicate that this"
                    " is a hard link (file), with LINK ID = %" PRIu32 "\n",
                    linkNum);
            return cnid;
        }

        if ((!hfs->has_root_crtime) || (!hfs->has_meta_crtime)) {
            if (tsk_verbose)
                tsk_fprintf(stderr,
                    "WARNING: hfs_follow_hard_link: Either the root folder or the"
                    " file metadata folder is not accessible.  Testing this potential hard link"
                    " may be impaired.\n");
        }

        if ((hfs->has_meta_crtime && (crtime == hfs->meta_crtime)) ||
            (hfs->has_meta_dir_crtime && (crtime == hfs->metadir_crtime))
            || (hfs->has_root_crtime && (crtime == hfs->root_crtime))) {
            uint32_t linkNum =
                tsk_getu32(fs->endian, cat->std.perm.special.inum);

            return linkNum;
        }
    }
    else if (file_type == HFS_LINKDIR_FILE_TYPE
        && file_creator == HFS_LINKDIR_FILE_CREATOR) {

        if (hfs->meta_dir_inum == 0)
            return cnid;

        if ((!hfs->has_root_crtime) && (!hfs->has_meta_dir_crtime)
            && (!hfs->has_meta_crtime)) {
            uint32_t linkNum =
                tsk_getu32(fs->endian, cat->std.perm.special.inum);
            *is_error = 1;

            if (tsk_verbose)
                tsk_fprintf(stderr,
                    "WARNING: hfs_follow_hard_link: File system creation times are not set. "
                    "Cannot test inode for hard link. File type and creator indicate that this"
                    " is a hard link (directory), with LINK ID = %" PRIu32
                    "\n", linkNum);
            return cnid;
        }

        if ((!hfs->has_root_crtime) || (!hfs->has_meta_crtime)
            || (!hfs->has_meta_dir_crtime)) {
            if (tsk_verbose)
                tsk_fprintf(stderr,
                    "WARNING: hfs_follow_hard_link: Either the root folder or the"
                    " file metadata folder or the directory metatdata folder is"
                    " not accessible.  Testing this potential hard linked folder "
                    "may be impaired.\n");
        }

        if ((hfs->has_meta_crtime && (crtime == hfs->meta_crtime)) ||
            (hfs->has_meta_dir_crtime && (crtime == hfs->metadir_crtime))
            || (hfs->has_root_crtime && (crtime == hfs->root_crtime))) {
            uint32_t linkNum =
                tsk_getu32(fs->endian, cat->std.perm.special.inum);

            return linkNum;
        }
    }

    return cnid;
}
