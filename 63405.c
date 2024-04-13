hfs_ext_find_extent_record_attr(HFS_INFO * hfs, uint32_t cnid,
    TSK_FS_ATTR * a_attr, unsigned char dataForkQ)
{
    TSK_FS_INFO *fs = (TSK_FS_INFO *) & (hfs->fs_info);
    uint16_t nodesize;          /* size of nodes (all, regardless of the name) */
    uint32_t cur_node;          /* node id of the current node */
    char *node = NULL;
    uint8_t is_done;
    uint8_t desiredType;

    tsk_error_reset();

    if (tsk_verbose)
        tsk_fprintf(stderr,
            "hfs_ext_find_extent_record_attr: Looking for extents for file %"
            PRIu32 " %s\n", cnid,
            dataForkQ ? "data fork" : "resource fork");

    if (!hfs->has_extents_file) {
        return 0;
    }

    desiredType =
        dataForkQ ? HFS_EXT_KEY_TYPE_DATA : HFS_EXT_KEY_TYPE_RSRC;

    if (hfs->extents_file == NULL) {
        ssize_t cnt;

        if ((hfs->extents_file =
                tsk_fs_file_open_meta(fs, NULL,
                    HFS_EXTENTS_FILE_ID)) == NULL) {
            return 1;
        }

        /* cache the data attribute */
        hfs->extents_attr =
            tsk_fs_attrlist_get(hfs->extents_file->meta->attr,
            TSK_FS_ATTR_TYPE_DEFAULT);
        if (!hfs->extents_attr) {
            tsk_error_errstr2_concat
                (" - Default Attribute not found in Extents File");
            return 1;
        }

        cnt = tsk_fs_attr_read(hfs->extents_attr, 14,
            (char *) &(hfs->extents_header),
            sizeof(hfs_btree_header_record), 0);
        if (cnt != sizeof(hfs_btree_header_record)) {
            if (cnt >= 0) {
                tsk_error_reset();
                tsk_error_set_errno(TSK_ERR_FS_READ);
            }
            tsk_error_set_errstr2
                ("hfs_ext_find_extent_record_attr: Error reading header");
            return 1;
        }
    }

    nodesize = tsk_getu16(fs->endian, hfs->extents_header.nodesize);
    if ((node = (char *) tsk_malloc(nodesize)) == NULL) {
        return 1;
    }

    /* start at root node */
    cur_node = tsk_getu32(fs->endian, hfs->extents_header.rootNode);

    /* if the root node is zero, then the extents btree is empty */
    /* if no files have overflow extents, the Extents B-tree still
       exists on disk, but is an empty B-tree containing only
       the header node */
    if (cur_node == 0) {
        if (tsk_verbose)
            tsk_fprintf(stderr, "hfs_ext_find_extent_record: "
                "empty extents btree\n");
        free(node);
        return 0;
    }

    if (tsk_verbose)
        tsk_fprintf(stderr, "hfs_ext_find_extent_record: starting at "
            "root node %" PRIu32 "; nodesize = %"
            PRIu16 "\n", cur_node, nodesize);

    /* Recurse down to the needed leaf nodes and then go forward */
    is_done = 0;
    while (is_done == 0) {
        TSK_OFF_T cur_off;      /* start address of cur_node */
        uint16_t num_rec;       /* number of records in this node */
        ssize_t cnt;
        hfs_btree_node *node_desc;

        if (cur_node > tsk_getu32(fs->endian,
                hfs->extents_header.totalNodes)) {
            tsk_error_set_errno(TSK_ERR_FS_GENFS);
            tsk_error_set_errstr
                ("hfs_ext_find_extent_record_attr: Node %d too large for file",
                cur_node);
            free(node);
            return 1;
        }

        cur_off = (TSK_OFF_T)cur_node * nodesize;
        if (tsk_verbose)
            tsk_fprintf(stderr,
                "hfs_ext_find_extent_record: reading node %" PRIu32
                " at offset %" PRIuOFF "\n", cur_node, cur_off);

        cnt = tsk_fs_attr_read(hfs->extents_attr, cur_off,
            node, nodesize, 0);
        if (cnt != nodesize) {
            if (cnt >= 0) {
                tsk_error_reset();
                tsk_error_set_errno(TSK_ERR_FS_READ);
            }
            tsk_error_set_errstr2
                ("hfs_ext_find_extent_record_attr: Error reading node %d at offset %"
                PRIuOFF, cur_node, cur_off);
            free(node);
            return 1;
        }

        if (nodesize < sizeof(hfs_btree_node)) {
            tsk_error_set_errno(TSK_ERR_FS_GENFS);
            tsk_error_set_errstr
                ("hfs_ext_find_extent_record_attr: Node size %d is too small to be valid", nodesize);
            free(node);
            return 1;
        }
        node_desc = (hfs_btree_node *) node;
        num_rec = tsk_getu16(fs->endian, node_desc->num_rec);

        if (num_rec == 0) {
            tsk_error_set_errno(TSK_ERR_FS_GENFS);
            tsk_error_set_errstr
                ("hfs_ext_find_extent_record: zero records in node %"
                PRIu32, cur_node);
            free(node);
            return 1;
        }


        /* With an index node, find the record with the largest key that is smaller
         * to or equal to cnid */
        if (node_desc->type == HFS_BT_NODE_TYPE_IDX) {
            uint32_t next_node = 0;
            int rec;

            if (tsk_verbose)
                tsk_fprintf(stderr,
                    "hfs_ext_find_extent_record: Index node %" PRIu32
                    " @ %" PRIu64 " has %" PRIu16 " records\n", cur_node,
                    cur_off, num_rec);

            for (rec = 0; rec < num_rec; ++rec) {
                int cmp;
                size_t rec_off;
                hfs_btree_key_ext *key;

                rec_off =
                    tsk_getu16(fs->endian,
                    &node[nodesize - (rec + 1) * 2]);
                if (rec_off + sizeof(hfs_btree_key_ext) > nodesize) {
                    tsk_error_set_errno(TSK_ERR_FS_GENFS);
                    tsk_error_set_errstr
                        ("hfs_ext_find_extent_record_attr: offset of record %d in index node %d too large (%d vs %"
                        PRIu16 ")", rec, cur_node, (int) rec_off,
                        nodesize);
                    free(node);
                    return 1;
                }
                key = (hfs_btree_key_ext *) & node[rec_off];

                cmp = hfs_ext_compare_keys(hfs, cnid, key);

                if (tsk_verbose)
                    tsk_fprintf(stderr,
                        "hfs_ext_find_extent_record: record %" PRIu16
                        " ; keylen %" PRIu16 " (FileId: %" PRIu32
                        ", ForkType: %" PRIu8 ", StartBlk: %" PRIu32
                        "); compare: %d\n", rec, tsk_getu16(fs->endian,
                            key->key_len), tsk_getu32(fs->endian,
                            key->file_id), key->fork_type,
                        tsk_getu32(fs->endian, key->start_block), cmp);

                /* save the info from this record unless it is bigger than cnid */
                if ((cmp <= 0) || (next_node == 0)) {
                    hfs_btree_index_record *idx_rec;
                    int keylen =
                        2 + hfs_get_idxkeylen(hfs, tsk_getu16(fs->endian,
                            key->key_len), &(hfs->extents_header));
                    if (rec_off + keylen > nodesize) {
                        tsk_error_set_errno(TSK_ERR_FS_GENFS);
                        tsk_error_set_errstr
                            ("hfs_ext_find_extent_record_attr: offset and keylenth of record %d in index node %d too large (%d vs %"
                            PRIu16 ")", rec, cur_node,
                            (int) rec_off + keylen, nodesize);
                        free(node);
                        return 1;
                    }
                    idx_rec =
                        (hfs_btree_index_record *) & node[rec_off +
                        keylen];
                    next_node = tsk_getu32(fs->endian, idx_rec->childNode);
                }

                if (cmp > 0) {
                    break;
                }
            }

            if (next_node == 0) {
                if (tsk_verbose)
                    tsk_fprintf(stderr,
                        "hfs_ext_find_extent_record_attr: did not find any keys for %d in index node %d",
                        cnid, cur_node);
                is_done = 1;
                break;
            }
            cur_node = next_node;
        }

        /* with a leaf, we process until we are past cnid.  We move right too if we can */
        else if (node_desc->type == HFS_BT_NODE_TYPE_LEAF) {
            int rec;

            if (tsk_verbose)
                tsk_fprintf(stderr,
                    "hfs_ext_find_extent_record: Leaf node %" PRIu32 " @ %"
                    PRIu64 " has %" PRIu16 " records\n", cur_node, cur_off,
                    num_rec);

            for (rec = 0; rec < num_rec; ++rec) {
                size_t rec_off;
                hfs_btree_key_ext *key;
                uint32_t rec_cnid;
                hfs_extents *extents;
                TSK_OFF_T ext_off = 0;
                int keylen;
                TSK_FS_ATTR_RUN *attr_run;

                rec_off =
                    tsk_getu16(fs->endian,
                    &node[nodesize - (rec + 1) * 2]);
                if (rec_off > nodesize) {
                    tsk_error_set_errno(TSK_ERR_FS_GENFS);
                    tsk_error_set_errstr
                        ("hfs_ext_find_extent_record_attr: offset of record %d in leaf node %d too large (%d vs %"
                        PRIu16 ")", rec, cur_node, (int) rec_off,
                        nodesize);
                    free(node);
                    return 1;
                }
                key = (hfs_btree_key_ext *) & node[rec_off];

                if (tsk_verbose)
                    tsk_fprintf(stderr,
                        "hfs_ext_find_extent_record: record %" PRIu16
                        "; keylen %" PRIu16 " (%" PRIu32
                        ", %" PRIu8 ", %" PRIu32 ")\n", rec,
                        tsk_getu16(fs->endian, key->key_len),
                        tsk_getu32(fs->endian, key->file_id),
                        key->fork_type, tsk_getu32(fs->endian,
                            key->start_block));

                rec_cnid = tsk_getu32(fs->endian, key->file_id);


                if (rec_cnid < cnid) {
                    continue;
                }
                if (rec_cnid > cnid) {
                    is_done = 1;
                    break;
                }


                if (key->fork_type != desiredType) {
                    if (dataForkQ) {
                        is_done = 1;
                        break;
                    }
                    else
                        continue;
                }

                keylen = 2 + tsk_getu16(fs->endian, key->key_len);
                if (rec_off + keylen + sizeof(hfs_extents) > nodesize) {
                    tsk_error_set_errno(TSK_ERR_FS_GENFS);
                    tsk_error_set_errstr
                        ("hfs_ext_find_extent_record_attr: offset and keylenth of record %d in leaf node %d too large (%d vs %"
                        PRIu16 ")", rec, cur_node, (int) rec_off + keylen,
                        nodesize);
                    free(node);
                    return 1;
                }

                ext_off = tsk_getu32(fs->endian, key->start_block);

                extents = (hfs_extents *) & node[rec_off + keylen];

                attr_run =
                    hfs_extents_to_attr(fs, extents->extents, ext_off);
                if ((attr_run == NULL) && (tsk_error_get_errno() != 0)) {
                    tsk_error_errstr2_concat
                        (" - hfs_ext_find_extent_record_attr");
                    free(node);
                    return 1;
                }

                if (tsk_fs_attr_add_run(fs, a_attr, attr_run)) {
                    tsk_error_errstr2_concat
                        (" - hfs_ext_find_extent_record_attr");
                    free(node);
                    return 1;
                }
            }
            cur_node = tsk_getu32(fs->endian, node_desc->flink);
            if (cur_node == 0) {
                is_done = 1;
                break;
            }
        }
        else {
            tsk_error_set_errno(TSK_ERR_FS_GENFS);
            tsk_error_set_errstr("hfs_ext_find_extent_record: btree node %"
                PRIu32 " (%" PRIuOFF ") is neither index nor leaf (%" PRIu8
                ")", cur_node, cur_off, node_desc->type);
            free(node);
            return 1;
        }
    }
    free(node);
    return 0;
}
