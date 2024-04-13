static void _store_resource_sdb(struct PE_(r_bin_pe_obj_t) *bin) {
	RListIter *iter;
	r_pe_resource *rs;
	int index = 0;
	ut64 vaddr = 0;
	char *key;
	Sdb *sdb = sdb_new0 ();
	if (!sdb) {
		return;
	}
	r_list_foreach (bin->resources, iter, rs) {
		key = sdb_fmt ("resource.%d.timestr", index);
		sdb_set (sdb, key, rs->timestr, 0);
		key = sdb_fmt ("resource.%d.vaddr", index);
		vaddr = bin_pe_rva_to_va (bin, rs->data->OffsetToData);
		sdb_num_set (sdb, key, vaddr, 0);
		key = sdb_fmt ("resource.%d.name", index);
		sdb_num_set (sdb, key, rs->name, 0);
		key = sdb_fmt ("resource.%d.size", index);
		sdb_num_set (sdb, key, rs->data->Size, 0);
		key = sdb_fmt ("resource.%d.type", index);
		sdb_set (sdb, key, rs->type, 0);
		key = sdb_fmt ("resource.%d.language", index);
		sdb_set (sdb, key, rs->language, 0);
		index++;
	}
	sdb_ns_set (bin->kv, "pe_resource", sdb);
}
