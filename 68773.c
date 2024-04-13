struct page *get_node_page(struct f2fs_sb_info *sbi, pgoff_t nid)
{
	return __get_node_page(sbi, nid, NULL, 0);
}
