R_API void PE_(bin_pe_parse_resource)(struct PE_(r_bin_pe_obj_t) *bin) {
	int index = 0;
	ut64 off = 0, rsrc_base = bin->resource_directory_offset;
	Pe_image_resource_directory *rs_directory = bin->resource_directory;
	ut32 curRes = 0;
	int totalRes = 0;
	SdbHash *dirs = sdb_ht_new (); //to avoid infinite loops
	if (!dirs) {
		return;
	}
	if (!rs_directory) {
		sdb_ht_free (dirs);
		return;
	}
	curRes = rs_directory->NumberOfNamedEntries;
	totalRes = curRes + rs_directory->NumberOfIdEntries;
	if (totalRes > R_PE_MAX_RESOURCES) {
		eprintf ("Error parsing resource directory\n");
		sdb_ht_free (dirs);
		return;
	}
	for (index = 0; index < totalRes; index++) {
		Pe_image_resource_directory_entry typeEntry;
		off = rsrc_base + sizeof (*rs_directory) + index * sizeof (typeEntry);
		sdb_ht_insert (dirs, sdb_fmt ("0x%08"PFMT64x, off), "1");
		if (off > bin->size || off + sizeof(typeEntry) > bin->size) {
			break;
		}
		if (r_buf_read_at (bin->b, off, (ut8*)&typeEntry, sizeof(typeEntry)) < 1) {
			eprintf ("Warning: read resource  directory entry\n");
			break;
		}
		if (typeEntry.u2.s.DataIsDirectory) {
			Pe_image_resource_directory identEntry;
			off = rsrc_base + typeEntry.u2.s.OffsetToDirectory;
			int len = r_buf_read_at (bin->b, off, (ut8*)&identEntry, sizeof(identEntry));
			if (len < 1 || len != sizeof (identEntry)) {
				eprintf ("Warning: parsing resource directory\n");
			}
			_parse_resource_directory (bin, &identEntry, typeEntry.u2.s.OffsetToDirectory, typeEntry.u1.Id, 0, dirs);
		}
	}
	sdb_ht_free (dirs);
	_store_resource_sdb (bin);
}
