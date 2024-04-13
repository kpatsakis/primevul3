hfs_fsstat(TSK_FS_INFO * fs, FILE * hFile)
{
    HFS_INFO *hfs = (HFS_INFO *) fs;
    hfs_plus_vh *sb = hfs->fs;
    time_t mac_time;
    TSK_INUM_T inode;
    char timeBuf[128];

    if (tsk_verbose)
        tsk_fprintf(stderr, "hfs_fstat: called\n");

    tsk_fprintf(hFile, "FILE SYSTEM INFORMATION\n");
    tsk_fprintf(hFile, "--------------------------------------------\n");

    tsk_fprintf(hFile, "File System Type: ");
    if (tsk_getu16(fs->endian, hfs->fs->signature) == HFS_VH_SIG_HFSPLUS)
        tsk_fprintf(hFile, "HFS+\n");
    else if (tsk_getu16(fs->endian, hfs->fs->signature) == HFS_VH_SIG_HFSX)
        tsk_fprintf(hFile, "HFSX\n");
    else
        tsk_fprintf(hFile, "Unknown\n");

    tsk_fprintf(hFile, "File System Version: ");
    switch (tsk_getu16(fs->endian, hfs->fs->version)) {
    case 4:
        tsk_fprintf(hFile, "HFS+\n");
        break;
    case 5:
        tsk_fprintf(hFile, "HFSX\n");
        break;
    default:
        tsk_fprintf(hFile, "Unknown (%" PRIu16 ")\n",
            tsk_getu16(fs->endian, hfs->fs->version));
        break;
    }

    if (tsk_getu16(fs->endian, hfs->fs->signature) == HFS_VH_SIG_HFSX) {
        tsk_fprintf(hFile, "Case Sensitive: %s\n",
            hfs->is_case_sensitive ? "yes" : "no");
    }

    if (hfs->hfs_wrapper_offset > 0) {
        tsk_fprintf(hFile,
            "File system is embedded in an HFS wrapper at offset %" PRIuOFF
            "\n", hfs->hfs_wrapper_offset);
    }

    tsk_fprintf(hFile, "\nVolume Name: ");
    if (print_inode_name(hFile, fs, HFS_ROOT_INUM))
        return 1;
    tsk_fprintf(hFile, "\n");

    tsk_fprintf(hFile, "Volume Identifier: %08" PRIx32 "%08" PRIx32 "\n",
        tsk_getu32(fs->endian, sb->finder_info[HFS_VH_FI_ID1]),
        tsk_getu32(fs->endian, sb->finder_info[HFS_VH_FI_ID2]));


    tsk_fprintf(hFile, "\nLast Mounted By: ");
    if (tsk_getu32(fs->endian, sb->last_mnt_ver) == HFS_VH_MVER_HFSPLUS)
        tsk_fprintf(hFile, "Mac OS X\n");
    else if (tsk_getu32(fs->endian, sb->last_mnt_ver) == HFS_VH_MVER_HFSJ)
        tsk_fprintf(hFile, "Mac OS X, Journaled\n");
    else if (tsk_getu32(fs->endian, sb->last_mnt_ver) == HFS_VH_MVER_FSK)
        tsk_fprintf(hFile, "failed journal replay\n");
    else if (tsk_getu32(fs->endian, sb->last_mnt_ver) == HFS_VH_MVER_FSCK)
        tsk_fprintf(hFile, "fsck_hfs\n");
    else if (tsk_getu32(fs->endian, sb->last_mnt_ver) == HFS_VH_MVER_OS89)
        tsk_fprintf(hFile, "Mac OS 8.1 - 9.2.2\n");
    else
        tsk_fprintf(hFile, "Unknown (%" PRIx32 "\n",
            tsk_getu32(fs->endian, sb->last_mnt_ver));

    /* State of the file system */
    if ((tsk_getu32(fs->endian, hfs->fs->attr) & HFS_VH_ATTR_UNMOUNTED)
        && (!(tsk_getu32(fs->endian,
                    hfs->fs->attr) & HFS_VH_ATTR_INCONSISTENT)))
        tsk_fprintf(hFile, "Volume Unmounted Properly\n");
    else
        tsk_fprintf(hFile, "Volume Unmounted Improperly\n");

    tsk_fprintf(hFile, "Mount Count: %" PRIu32 "\n",
        tsk_getu32(fs->endian, sb->write_cnt));


    mac_time =
        hfs_convert_2_unix_time(tsk_getu32(fs->endian, hfs->fs->cr_date));
    tsk_fprintf(hFile, "\nCreation Date: \t%s\n",
        tsk_fs_time_to_str(mktime(gmtime(&mac_time)), timeBuf));

    mac_time =
        hfs_convert_2_unix_time(tsk_getu32(fs->endian, hfs->fs->m_date));
    tsk_fprintf(hFile, "Last Written Date: \t%s\n",
        tsk_fs_time_to_str(mac_time, timeBuf));

    mac_time =
        hfs_convert_2_unix_time(tsk_getu32(fs->endian,
            hfs->fs->bkup_date));
    tsk_fprintf(hFile, "Last Backup Date: \t%s\n",
        tsk_fs_time_to_str(mac_time, timeBuf));

    mac_time =
        hfs_convert_2_unix_time(tsk_getu32(fs->endian, hfs->fs->chk_date));
    tsk_fprintf(hFile, "Last Checked Date: \t%s\n",
        tsk_fs_time_to_str(mac_time, timeBuf));


    if (tsk_getu32(fs->endian, hfs->fs->attr) & HFS_VH_ATTR_SOFTWARE_LOCK)
        tsk_fprintf(hFile, "Software write protect enabled\n");

    /* Print journal information */
    if (tsk_getu32(fs->endian, sb->attr) & HFS_VH_ATTR_JOURNALED) {
        tsk_fprintf(hFile, "\nJournal Info Block: %" PRIu32 "\n",
            tsk_getu32(fs->endian, sb->jinfo_blk));
    }

    tsk_fprintf(hFile, "\nMETADATA INFORMATION\n");
    tsk_fprintf(hFile, "--------------------------------------------\n");

    tsk_fprintf(hFile, "Range: %" PRIuINUM " - %" PRIuINUM "\n",
        fs->first_inum, fs->last_inum);

    inode = tsk_getu32(fs->endian, sb->finder_info[HFS_VH_FI_BOOT]);
    tsk_fprintf(hFile, "Bootable Folder ID: %" PRIuINUM, inode);
    if (inode > 0)
        print_inode_file(hFile, fs, inode);
    tsk_fprintf(hFile, "\n");

    inode = tsk_getu32(fs->endian, sb->finder_info[HFS_VH_FI_START]);
    tsk_fprintf(hFile, "Startup App ID: %" PRIuINUM, inode);
    if (inode > 0)
        print_inode_file(hFile, fs, inode);
    tsk_fprintf(hFile, "\n");

    inode = tsk_getu32(fs->endian, sb->finder_info[HFS_VH_FI_OPEN]);
    tsk_fprintf(hFile, "Startup Open Folder ID: %" PRIuINUM, inode);
    if (inode > 0)
        print_inode_file(hFile, fs, inode);
    tsk_fprintf(hFile, "\n");

    inode = tsk_getu32(fs->endian, sb->finder_info[HFS_VH_FI_BOOT9]);
    tsk_fprintf(hFile, "Mac OS 8/9 Blessed System Folder ID: %" PRIuINUM,
        inode);
    if (inode > 0)
        print_inode_file(hFile, fs, inode);
    tsk_fprintf(hFile, "\n");

    inode = tsk_getu32(fs->endian, sb->finder_info[HFS_VH_FI_BOOTX]);
    tsk_fprintf(hFile, "Mac OS X Blessed System Folder ID: %" PRIuINUM,
        inode);
    if (inode > 0)
        print_inode_file(hFile, fs, inode);
    tsk_fprintf(hFile, "\n");

    tsk_fprintf(hFile, "Number of files: %" PRIu32 "\n",
        tsk_getu32(fs->endian, sb->file_cnt));
    tsk_fprintf(hFile, "Number of folders: %" PRIu32 "\n",
        tsk_getu32(fs->endian, sb->fldr_cnt));


    tsk_fprintf(hFile, "\nCONTENT INFORMATION\n");
    tsk_fprintf(hFile, "--------------------------------------------\n");

    tsk_fprintf(hFile, "Block Range: %" PRIuDADDR " - %" PRIuDADDR "\n",
        fs->first_block, fs->last_block);

    if (fs->last_block != fs->last_block_act)
        tsk_fprintf(hFile,
            "Total Range in Image: %" PRIuDADDR " - %" PRIuDADDR "\n",
            fs->first_block, fs->last_block_act);

    tsk_fprintf(hFile, "Allocation Block Size: %u\n", fs->block_size);

    tsk_fprintf(hFile, "Number of Free Blocks: %" PRIu32 "\n",
        tsk_getu32(fs->endian, sb->free_blks));

    if (tsk_getu32(fs->endian, hfs->fs->attr) & HFS_VH_ATTR_BADBLOCKS)
        tsk_fprintf(hFile, "Volume has bad blocks\n");

    return 0;
}
