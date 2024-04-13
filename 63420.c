hfs_istat(TSK_FS_INFO * fs, TSK_FS_ISTAT_FLAG_ENUM istat_flags, FILE * hFile, TSK_INUM_T inum,
    TSK_DADDR_T numblock, int32_t sec_skew)
{
    HFS_INFO *hfs = (HFS_INFO *) fs;
    TSK_FS_FILE *fs_file;
    char hfs_mode[12];
    HFS_PRINT_ADDR print;
    HFS_ENTRY entry;
    char timeBuf[128];
    const TSK_FS_ATTR *compressionAttr = NULL;
    RES_DESCRIPTOR *rd;         // descriptor of a resource

    tsk_error_reset();

    if (tsk_verbose)
        tsk_fprintf(stderr,
            "hfs_istat: inum: %" PRIuINUM " numblock: %" PRIu32 "\n",
            inum, numblock);

    if ((fs_file = tsk_fs_file_open_meta(fs, NULL, inum)) == NULL) {
        error_returned("hfs_istat: getting metadata for the file");
        return 1;
    }

    if (inum >= HFS_FIRST_USER_CNID) {
        int rslt;
        tsk_fprintf(hFile, "File Path: ");
        rslt = print_parent_path(hFile, fs, inum);
        if (rslt != 0)
            tsk_fprintf(hFile, " Error in printing path\n");
        else
            tsk_fprintf(hFile, "\n");
    }
    else {
        if (fs_file->meta->name2 != NULL)
            tsk_fprintf(hFile, "File Name: %s\n",
                fs_file->meta->name2->name);
    }

    tsk_fprintf(hFile, "Catalog Record: %" PRIuINUM "\n", inum);
    tsk_fprintf(hFile, "%sAllocated\n",
        (fs_file->meta->flags & TSK_FS_META_FLAG_UNALLOC) ? "Not " : "");

    tsk_fprintf(hFile, "Type:\t");
    if (fs_file->meta->type == TSK_FS_META_TYPE_REG)
        tsk_fprintf(hFile, "File\n");
    else if (TSK_FS_IS_DIR_META(fs_file->meta->type))
        tsk_fprintf(hFile, "Folder\n");
    else
        tsk_fprintf(hFile, "\n");

    tsk_fs_meta_make_ls(fs_file->meta, hfs_mode, sizeof(hfs_mode));
    tsk_fprintf(hFile, "Mode:\t%s\n", hfs_mode);
    tsk_fprintf(hFile, "Size:\t%" PRIuOFF "\n", fs_file->meta->size);

    if (fs_file->meta->link)
        tsk_fprintf(hFile, "Symbolic link to:\t%s\n", fs_file->meta->link);

    tsk_fprintf(hFile, "uid / gid: %" PRIuUID " / %" PRIuGID "\n",
        fs_file->meta->uid, fs_file->meta->gid);

    tsk_fprintf(hFile, "Link count:\t%d\n", fs_file->meta->nlink);

    if (hfs_cat_file_lookup(hfs, inum, &entry, TRUE) == 0) {
        hfs_uni_str *nm = &entry.thread.name;
        char name_buf[HFS_MAXNAMLEN + 1];
        TSK_INUM_T par_cnid;    // parent CNID

        tsk_fprintf(hFile, "\n");
        hfs_UTF16toUTF8(fs, nm->unicode, (int) tsk_getu16(fs->endian,
                nm->length), &name_buf[0], HFS_MAXNAMLEN + 1,
            HFS_U16U8_FLAG_REPLACE_SLASH | HFS_U16U8_FLAG_REPLACE_CONTROL);
        tsk_fprintf(hFile, "File Name: %s\n", name_buf);

        par_cnid = tsk_getu32(fs->endian, &(entry.thread.parent_cnid));
        if ((hfs->has_meta_dir_crtime && par_cnid == hfs->meta_dir_inum) ||
            (hfs->has_meta_crtime && par_cnid == hfs->meta_inum)) {
            int instr = strncmp(name_buf, "iNode", 5);
            int drstr = strncmp(name_buf, "dir_", 4);

            if (instr == 0 &&
                hfs->has_meta_crtime && par_cnid == hfs->meta_inum) {
                tsk_fprintf(hFile, "This is a hard link to a file\n");
            }
            else if (drstr == 0 &&
                hfs->has_meta_dir_crtime &&
                par_cnid == hfs->meta_dir_inum) {
                tsk_fprintf(hFile, "This is a hard link to a folder.\n");
            }
        }

        /* The cat.perm union contains file-type specific values.
         * Print them if they are relevant. */
        if ((fs_file->meta->type == TSK_FS_META_TYPE_CHR) ||
            (fs_file->meta->type == TSK_FS_META_TYPE_BLK)) {
            tsk_fprintf(hFile, "Device ID:\t%" PRIu32 "\n",
                tsk_getu32(fs->endian, entry.cat.std.perm.special.raw));
        }
        else if ((tsk_getu32(fs->endian,
                    entry.cat.std.u_info.file_type) ==
                HFS_HARDLINK_FILE_TYPE)
            && (tsk_getu32(fs->endian,
                    entry.cat.std.u_info.file_cr) ==
                HFS_HARDLINK_FILE_CREATOR)) {
            tsk_fprintf(hFile, "Hard link inode number\t %" PRIu32 "\n",
                tsk_getu32(fs->endian, entry.cat.std.perm.special.inum));
        }

        tsk_fprintf(hFile, "Admin flags: %" PRIu8,
            entry.cat.std.perm.a_flags);
        if (entry.cat.std.perm.a_flags != 0) {
            tsk_fprintf(hFile, " - ");
            if (entry.cat.std.perm.a_flags & HFS_PERM_AFLAG_ARCHIVED)
                tsk_fprintf(hFile, "archived ");
            if (entry.cat.std.perm.a_flags & HFS_PERM_AFLAG_IMMUTABLE)
                tsk_fprintf(hFile, "immutable ");
            if (entry.cat.std.perm.a_flags & HFS_PERM_AFLAG_APPEND)
                tsk_fprintf(hFile, "append-only ");
        }
        tsk_fprintf(hFile, "\n");

        tsk_fprintf(hFile, "Owner flags: %" PRIu8,
            entry.cat.std.perm.o_flags);
        if (entry.cat.std.perm.o_flags != 0) {
            tsk_fprintf(hFile, " - ");
            if (entry.cat.std.perm.o_flags & HFS_PERM_OFLAG_NODUMP)
                tsk_fprintf(hFile, "no-dump ");
            if (entry.cat.std.perm.o_flags & HFS_PERM_OFLAG_IMMUTABLE)
                tsk_fprintf(hFile, "immutable ");
            if (entry.cat.std.perm.o_flags & HFS_PERM_OFLAG_APPEND)
                tsk_fprintf(hFile, "append-only ");
            if (entry.cat.std.perm.o_flags & HFS_PERM_OFLAG_OPAQUE)
                tsk_fprintf(hFile, "opaque ");
            if (entry.cat.std.perm.o_flags & HFS_PERM_OFLAG_COMPRESSED)
                tsk_fprintf(hFile, "compressed ");
        }
        tsk_fprintf(hFile, "\n");

        if (tsk_getu16(fs->endian,
                entry.cat.std.flags) & HFS_FILE_FLAG_LOCKED)
            tsk_fprintf(hFile, "Locked\n");
        if (tsk_getu16(fs->endian,
                entry.cat.std.flags) & HFS_FILE_FLAG_ATTR)
            tsk_fprintf(hFile, "Has extended attributes\n");
        if (tsk_getu16(fs->endian,
                entry.cat.std.flags) & HFS_FILE_FLAG_ACL)
            tsk_fprintf(hFile, "Has security data (ACLs)\n");

        if ( !TSK_FS_IS_DIR_META(fs_file->meta->type)){
            int windx;          // loop index
            tsk_fprintf(hFile,
                "File type:\t%04" PRIx32 "  ",
                tsk_getu32(fs->endian, entry.cat.std.u_info.file_type));

            for (windx = 0; windx < 4; ++windx) {
                uint8_t cu = entry.cat.std.u_info.file_type[windx];
                if (cu >= 32 && cu <= 126)
                    tsk_fprintf(hFile, "%c", (char) cu);
                else
                    tsk_fprintf(hFile, " ");
            }
            tsk_fprintf(hFile, "\n");
            tsk_fprintf(hFile,
                "File creator:\t%04" PRIx32 "  ",
                tsk_getu32(fs->endian, entry.cat.std.u_info.file_cr));
            for (windx = 0; windx < 4; ++windx) {
                uint8_t cu = entry.cat.std.u_info.file_cr[windx];
                if (cu >= 32 && cu <= 126)
                    tsk_fprintf(hFile, "%c", (char) cu);
                else
                    tsk_fprintf(hFile, " ");
            }
            tsk_fprintf(hFile, "\n");
        }                       // END if(not folder)

        if (tsk_getu16(fs->endian,
                entry.cat.std.u_info.flags) & HFS_FINDER_FLAG_NAME_LOCKED)
            tsk_fprintf(hFile, "Name locked\n");
        if (tsk_getu16(fs->endian,
                entry.cat.std.u_info.flags) & HFS_FINDER_FLAG_HAS_BUNDLE)
            tsk_fprintf(hFile, "Has bundle\n");
        if (tsk_getu16(fs->endian,
                entry.cat.std.u_info.flags) & HFS_FINDER_FLAG_IS_INVISIBLE)
            tsk_fprintf(hFile, "Is invisible\n");
        if (tsk_getu16(fs->endian,
                entry.cat.std.u_info.flags) & HFS_FINDER_FLAG_IS_ALIAS)
            tsk_fprintf(hFile, "Is alias\n");

        tsk_fprintf(hFile, "Text encoding:\t%" PRIx32 " = %s\n",
            tsk_getu32(fs->endian, entry.cat.std.text_enc),
            text_encoding_name(tsk_getu32(fs->endian,
                    entry.cat.std.text_enc)));

        if (tsk_getu16(fs->endian,
                entry.cat.std.rec_type) == HFS_FILE_RECORD) {
            tsk_fprintf(hFile, "Resource fork size:\t%" PRIu64 "\n",
                tsk_getu64(fs->endian, entry.cat.resource.logic_sz));
        }
    }

    if (sec_skew != 0) {
        tsk_fprintf(hFile, "\nAdjusted times:\n");
        if (fs_file->meta->mtime)
            fs_file->meta->mtime -= sec_skew;
        if (fs_file->meta->atime)
            fs_file->meta->atime -= sec_skew;
        if (fs_file->meta->ctime)
            fs_file->meta->ctime -= sec_skew;
        if (fs_file->meta->crtime)
            fs_file->meta->crtime -= sec_skew;
        if (fs_file->meta->time2.hfs.bkup_time)
            fs_file->meta->time2.hfs.bkup_time -= sec_skew;

        tsk_fprintf(hFile, "Created:\t%s\n",
            tsk_fs_time_to_str(fs_file->meta->crtime, timeBuf));
        tsk_fprintf(hFile, "Content Modified:\t%s\n",
            tsk_fs_time_to_str(fs_file->meta->mtime, timeBuf));
        tsk_fprintf(hFile, "Attributes Modified:\t%s\n",
            tsk_fs_time_to_str(fs_file->meta->ctime, timeBuf));
        tsk_fprintf(hFile, "Accessed:\t%s\n",
            tsk_fs_time_to_str(fs_file->meta->atime, timeBuf));
        tsk_fprintf(hFile, "Backed Up:\t%s\n",
            tsk_fs_time_to_str(fs_file->meta->time2.hfs.bkup_time,
                timeBuf));

        if (fs_file->meta->mtime)
            fs_file->meta->mtime += sec_skew;
        if (fs_file->meta->atime)
            fs_file->meta->atime += sec_skew;
        if (fs_file->meta->ctime)
            fs_file->meta->ctime += sec_skew;
        if (fs_file->meta->crtime)
            fs_file->meta->crtime += sec_skew;
        if (fs_file->meta->time2.hfs.bkup_time)
            fs_file->meta->time2.hfs.bkup_time += sec_skew;

        tsk_fprintf(hFile, "\nOriginal times:\n");
    }
    else {
        tsk_fprintf(hFile, "\nTimes:\n");
    }

    tsk_fprintf(hFile, "Created:\t%s\n",
        tsk_fs_time_to_str(fs_file->meta->crtime, timeBuf));
    tsk_fprintf(hFile, "Content Modified:\t%s\n",
        tsk_fs_time_to_str(fs_file->meta->mtime, timeBuf));
    tsk_fprintf(hFile, "Attributes Modified:\t%s\n",
        tsk_fs_time_to_str(fs_file->meta->ctime, timeBuf));
    tsk_fprintf(hFile, "Accessed:\t%s\n",
        tsk_fs_time_to_str(fs_file->meta->atime, timeBuf));
    tsk_fprintf(hFile, "Backed Up:\t%s\n",
        tsk_fs_time_to_str(fs_file->meta->time2.hfs.bkup_time, timeBuf));

    if (tsk_getu16(fs->endian, entry.cat.std.rec_type) == HFS_FILE_RECORD) {
        if (!(entry.cat.std.perm.o_flags & HFS_PERM_OFLAG_COMPRESSED)) {

            if (!(istat_flags & TSK_FS_ISTAT_RUNLIST)) {
                tsk_fprintf(hFile, "\nData Fork Blocks:\n");
                print.idx = 0;
                print.hFile = hFile;
                print.accumulating = FALSE;
                print.startBlock = 0;
                print.blockCount = 0;

                if (tsk_fs_file_walk_type(fs_file,
                    TSK_FS_ATTR_TYPE_HFS_DATA, HFS_FS_ATTR_ID_DATA,
                    (TSK_FS_FILE_WALK_FLAG_AONLY |
                        TSK_FS_FILE_WALK_FLAG_SLACK), print_addr_act,
                        (void *)&print)) {
                    tsk_fprintf(hFile, "\nError reading file data fork\n");
                    tsk_error_print(hFile);
                    tsk_error_reset();
                }
                else {
                    output_print_addr(&print);
                    if (print.idx != 0)
                        tsk_fprintf(hFile, "\n");
                }
            }
        }

        if (tsk_getu64(fs->endian, entry.cat.resource.logic_sz) > 0) {

            if (! (istat_flags & TSK_FS_ISTAT_RUNLIST)) {
                tsk_fprintf(hFile, "\nResource Fork Blocks:\n");

                print.idx = 0;
                print.hFile = hFile;
                print.accumulating = FALSE;
                print.startBlock = 0;
                print.blockCount = 0;

                if (tsk_fs_file_walk_type(fs_file,
                    TSK_FS_ATTR_TYPE_HFS_RSRC, HFS_FS_ATTR_ID_RSRC,
                    (TSK_FS_FILE_WALK_FLAG_AONLY |
                        TSK_FS_FILE_WALK_FLAG_SLACK), print_addr_act,
                        (void *)&print)) {
                    tsk_fprintf(hFile, "\nError reading file resource fork\n");
                    tsk_error_print(hFile);
                    tsk_error_reset();
                }
                else {
                    output_print_addr(&print);
                    if (print.idx != 0)
                        tsk_fprintf(hFile, "\n");
                }
            }
        }
    }

    (void) tsk_fs_file_attr_get(fs_file);

    /* Print all of the attributes */
    tsk_fprintf(hFile, "\nAttributes: \n");
    if (fs_file->meta->attr) {
        int cnt, i;

        cnt = tsk_fs_file_attr_getsize(fs_file);
        for (i = 0; i < cnt; ++i) {
            const char *type;   // type of the attribute as a string
            const TSK_FS_ATTR *fs_attr =
                tsk_fs_file_attr_get_idx(fs_file, i);
            if (!fs_attr)
                continue;

            type = hfs_attrTypeName((uint32_t) fs_attr->type);


            /* print the layout if it is non-resident and not "special" */
            if (fs_attr->flags & TSK_FS_ATTR_NONRES) {

                tsk_fprintf(hFile,
                    "Type: %s (%" PRIu32 "-%" PRIu16
                    ")   Name: %s   Non-Resident%s%s%s   size: %"
                    PRIuOFF "  init_size: %" PRIuOFF "\n", type,
                    fs_attr->type, fs_attr->id,
                    (fs_attr->name) ? fs_attr->name : "N/A",
                    (fs_attr->flags & TSK_FS_ATTR_ENC) ? ", Encrypted" :
                    "",
                    (fs_attr->flags & TSK_FS_ATTR_COMP) ? ", Compressed" :
                    "",
                    (fs_attr->flags & TSK_FS_ATTR_SPARSE) ? ", Sparse" :
                    "", fs_attr->size, fs_attr->nrd.initsize);

                if (istat_flags & TSK_FS_ISTAT_RUNLIST) {
                    if (tsk_fs_attr_print(fs_attr, hFile)) {
                        tsk_fprintf(hFile, "\nError creating run lists\n");
                        tsk_error_print(hFile);
                        tsk_error_reset();
                    }
                }
            }                   // END:  non-resident attribute case
            else {
                tsk_fprintf(hFile,
                    "Type: %s (%" PRIu32 "-%" PRIu16
                    ")   Name: %s   Resident%s%s%s   size: %"
                    PRIuOFF "\n",
                    type,
                    fs_attr->type,
                    fs_attr->id,
                    (fs_attr->name) ? fs_attr->name : "N/A",
                    (fs_attr->flags & TSK_FS_ATTR_ENC) ? ", Encrypted" :
                    "",
                    (fs_attr->flags & TSK_FS_ATTR_COMP) ? ", Compressed" :
                    "",
                    (fs_attr->flags & TSK_FS_ATTR_SPARSE) ? ", Sparse" :
                    "", fs_attr->size);
                if (fs_attr->type == TSK_FS_ATTR_TYPE_HFS_COMP_REC) {
                    if (compressionAttr == NULL) {
                        compressionAttr = fs_attr;
                    }
                    else {
                        error_detected(TSK_ERR_FS_CORRUPT,
                            "hfs_istat: more than one compression attribute");
                        return 1;
                    }
                }
            }                   // END: else (RESIDENT attribute case)
        }                       // END:  for(;;)  loop over attributes
    }                           // END:  if(fs_file->meta->attr is non-NULL)

    if ((entry.cat.std.perm.o_flags & HFS_PERM_OFLAG_COMPRESSED)
        && (compressionAttr == NULL))
        tsk_fprintf(hFile,
            "WARNING: Compression Flag is set, but there"
            " is no compression record for this file.\n");
    if (((entry.cat.std.perm.o_flags & HFS_PERM_OFLAG_COMPRESSED) == 0)
        && (compressionAttr != NULL))
        tsk_fprintf(hFile,
            "WARNING: Compression Flag is NOT set, but there"
            " is a compression record for this file.\n");

    if (compressionAttr != NULL) {
        const TSK_FS_ATTR *fs_attr = compressionAttr;
        ssize_t attrReadResult;
        DECMPFS_DISK_HEADER *cmph;
        uint32_t cmpType;
        uint64_t uncSize;
        uint64_t cmpSize = 0;

        char *aBuf = (char *) tsk_malloc((size_t) fs_attr->size);
        if (aBuf == NULL) {
            error_returned("hfs_istat: space for a compression attribute");
            return 1;
        }
        attrReadResult = tsk_fs_attr_read(fs_attr, (TSK_OFF_T) 0,
            aBuf, (size_t) fs_attr->size,
            (TSK_FS_FILE_READ_FLAG_ENUM) 0x00);
        if (attrReadResult == -1) {
            error_returned("hfs_istat: reading the compression attribute");
            free(aBuf);
            return 1;
        }
        else if (attrReadResult < fs_attr->size) {
            error_detected(TSK_ERR_FS_READ,
                "hfs_istat: could not read the whole compression attribute");
            free(aBuf);
            return 1;
        }
        cmph = (DECMPFS_DISK_HEADER *) aBuf;
        cmpType = tsk_getu32(TSK_LIT_ENDIAN, cmph->compression_type);
        uncSize = tsk_getu64(TSK_LIT_ENDIAN, cmph->uncompressed_size);

        tsk_fprintf(hFile, "\nCompressed File:\n");
        tsk_fprintf(hFile, "    Uncompressed size: %llu\n", uncSize);

        switch (cmpType) {
        case DECMPFS_TYPE_ZLIB_ATTR:
            {
                uint32_t off = (cmph->attr_bytes[0] & 0x0F) == 0x0F ? 17 : 16;
                cmpSize = fs_attr->size - off;

                tsk_fprintf(hFile,
                    "    Data follows compression record in the CMPF attribute\n"
                    "    %" PRIu64 " bytes of data at offset %u, %s compressed\n",
                    cmpSize, off, off == 16 ? "zlib" : "not");
            }
            break;

        case DECMPFS_TYPE_LZVN_ATTR:
            {
                uint32_t off = cmph->attr_bytes[0] == 0x06 ? 17 : 16;
                cmpSize = fs_attr->size - off;

                tsk_fprintf(hFile,
                    "    Data follows compression record in the CMPF attribute\n"
                    "    %" PRIu64 " bytes of data at offset %u, %s compressed\n",
                    cmpSize, off, off == 16 ? "lzvn" : "not");
            }
            break;

        case DECMPFS_TYPE_ZLIB_RSRC:
            tsk_fprintf(hFile,
                "    Data is zlib compressed in the resource fork\n");
            break;

        case DECMPFS_TYPE_LZVN_RSRC:
            tsk_fprintf(hFile,
                "    Data is lzvn compressed in the resource fork\n");
            break;

        default:
            tsk_fprintf(hFile, "    Compression type is %u: UNKNOWN\n",
                cmpType);
        }

        free(aBuf);

        if ((cmpType == DECMPFS_TYPE_ZLIB_RSRC ||
             cmpType == DECMPFS_TYPE_LZVN_RSRC)
            && (tsk_getu64(fs->endian, entry.cat.resource.logic_sz) == 0))
            tsk_fprintf(hFile,
                "WARNING: Compression record indicates compressed data"
                " in the RSRC Fork, but that fork is empty.\n");
    }

    rd = hfs_parse_resource_fork(fs_file);

    if (rd != NULL) {
        tsk_fprintf(hFile, "\nResources:\n");
        while (rd) {
            tsk_fprintf(hFile,
                "  Type: %s \tID: %-5u \tOffset: %-5u \tSize: %-5u \tName: %s\n",
                rd->type, rd->id, rd->offset, rd->length, rd->name);
            rd = rd->next;
        }
    }
    free_res_descriptor(rd);

    tsk_fs_file_close(fs_file);
    return 0;
}
