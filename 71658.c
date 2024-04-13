void btreeFree(struct BTREE *btree) {
	free(btree->records);
}
