hfs_cat_compare_keys(HFS_INFO * hfs, const hfs_btree_key_cat * key1,
    const hfs_btree_key_cat * key2)
{
    TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);
    uint32_t cnid1, cnid2;

    cnid1 = tsk_getu32(fs->endian, key1->parent_cnid);
    cnid2 = tsk_getu32(fs->endian, key2->parent_cnid);

    if (cnid1 < cnid2)
        return -1;
    if (cnid1 > cnid2)
        return 1;

    return hfs_unicode_compare(hfs, &key1->name, &key2->name);
}
