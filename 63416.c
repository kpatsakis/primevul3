hfs_get_default_attr_type(const TSK_FS_FILE * a_file)
{
    TSK_INUM_T inum = a_file->meta->addr;
    if (inum == 3 ||            // Extents File
        inum == 4 ||            // Catalog File
        inum == 5 ||            // Bad Blocks File
        inum == 6 ||            // Block Map (Allocation File)
        inum == 7 ||            // Startup File
        inum == 8 ||            // Attributes File
        inum == 14 ||           // Not sure if these two will actually work.  I don't see
        inum == 15)             // any code to load the attrs of these files, if they exist.
        return TSK_FS_ATTR_TYPE_DEFAULT;
    if (a_file->meta->type == TSK_FS_META_TYPE_REG ||
        a_file->meta->type == TSK_FS_META_TYPE_LNK)
        return TSK_FS_ATTR_TYPE_HFS_DATA;

    return TSK_FS_ATTR_TYPE_DEFAULT;
}
