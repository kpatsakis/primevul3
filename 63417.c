hfs_get_idxkeylen(HFS_INFO * hfs, uint16_t keylen,
    const hfs_btree_header_record * header)
{
    TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);

    if (tsk_getu32(fs->endian, header->attr) & HFS_BT_HEAD_ATTR_VARIDXKEYS)
        return keylen;
    else
        return tsk_getu16(fs->endian, header->maxKeyLen);
}
