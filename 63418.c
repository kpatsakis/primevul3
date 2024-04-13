hfs_get_inode_name(TSK_FS_INFO * fs, TSK_INUM_T inum)
{
    HFS_INFO *hfs = (HFS_INFO *) fs;
    HFS_ENTRY entry;
    char *fn = NULL;

    if (hfs_cat_file_lookup(hfs, inum, &entry, FALSE))
        return NULL;

    fn = malloc(HFS_MAXNAMLEN + 1);
    if (fn == NULL)
        return NULL;

    if (hfs_UTF16toUTF8(fs, entry.thread.name.unicode,
            tsk_getu16(fs->endian, entry.thread.name.length), fn,
            HFS_MAXNAMLEN + 1, HFS_U16U8_FLAG_REPLACE_SLASH)) {
        free(fn);
        return NULL;
    }

    return fn;
}
