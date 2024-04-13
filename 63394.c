hfs_cat_get_record_offset_cb(HFS_INFO * hfs, int8_t level_type,
    const hfs_btree_key_cat * cur_key,
    TSK_OFF_T key_off, void *ptr)
{
    HFS_CAT_GET_RECORD_OFFSET_DATA *offset_data = (HFS_CAT_GET_RECORD_OFFSET_DATA *)ptr;
    const hfs_btree_key_cat *targ_key = offset_data->targ_key;

    if (tsk_verbose)
        tsk_fprintf(stderr,
            "hfs_cat_get_record_offset_cb: %s node want: %" PRIu32
            " vs have: %" PRIu32 "\n",
            (level_type == HFS_BT_NODE_TYPE_IDX) ? "Index" : "Leaf",
            tsk_getu32(hfs->fs_info.endian, targ_key->parent_cnid),
            tsk_getu32(hfs->fs_info.endian, cur_key->parent_cnid));

    if (level_type == HFS_BT_NODE_TYPE_IDX) {
        int diff = hfs_cat_compare_keys(hfs, cur_key, targ_key);
        if (diff < 0)
            return HFS_BTREE_CB_IDX_LT;
        else
            return HFS_BTREE_CB_IDX_EQGT;
    }
    else {
        int diff = hfs_cat_compare_keys(hfs, cur_key, targ_key);

        if (diff < 0) {
            return HFS_BTREE_CB_LEAF_GO;
        }
        else if (diff == 0) {
            offset_data->off =
                key_off + 2 + tsk_getu16(hfs->fs_info.endian,
                cur_key->key_len);
        }
        return HFS_BTREE_CB_LEAF_STOP;
    }
}
