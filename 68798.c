static int truncate_nodes(struct dnode_of_data *dn, unsigned int nofs,
						int ofs, int depth)
{
	struct dnode_of_data rdn = *dn;
	struct page *page;
	struct f2fs_node *rn;
	nid_t child_nid;
	unsigned int child_nofs;
	int freed = 0;
	int i, ret;

	if (dn->nid == 0)
		return NIDS_PER_BLOCK + 1;

	trace_f2fs_truncate_nodes_enter(dn->inode, dn->nid, dn->data_blkaddr);

	page = get_node_page(F2FS_I_SB(dn->inode), dn->nid);
	if (IS_ERR(page)) {
		trace_f2fs_truncate_nodes_exit(dn->inode, PTR_ERR(page));
		return PTR_ERR(page);
	}

	ra_node_pages(page, ofs, NIDS_PER_BLOCK);

	rn = F2FS_NODE(page);
	if (depth < 3) {
		for (i = ofs; i < NIDS_PER_BLOCK; i++, freed++) {
			child_nid = le32_to_cpu(rn->in.nid[i]);
			if (child_nid == 0)
				continue;
			rdn.nid = child_nid;
			ret = truncate_dnode(&rdn);
			if (ret < 0)
				goto out_err;
			if (set_nid(page, i, 0, false))
				dn->node_changed = true;
		}
	} else {
		child_nofs = nofs + ofs * (NIDS_PER_BLOCK + 1) + 1;
		for (i = ofs; i < NIDS_PER_BLOCK; i++) {
			child_nid = le32_to_cpu(rn->in.nid[i]);
			if (child_nid == 0) {
				child_nofs += NIDS_PER_BLOCK + 1;
				continue;
			}
			rdn.nid = child_nid;
			ret = truncate_nodes(&rdn, child_nofs, 0, depth - 1);
			if (ret == (NIDS_PER_BLOCK + 1)) {
				if (set_nid(page, i, 0, false))
					dn->node_changed = true;
				child_nofs += ret;
			} else if (ret < 0 && ret != -ENOENT) {
				goto out_err;
			}
		}
		freed = child_nofs;
	}

	if (!ofs) {
		/* remove current indirect node */
		dn->node_page = page;
		truncate_node(dn);
		freed++;
	} else {
		f2fs_put_page(page, 1);
	}
	trace_f2fs_truncate_nodes_exit(dn->inode, freed);
	return freed;

out_err:
	f2fs_put_page(page, 1);
	trace_f2fs_truncate_nodes_exit(dn->inode, ret);
	return ret;
}
