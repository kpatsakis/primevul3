static bool parse_signature(struct MACH0_(obj_t) *bin, ut64 off) {
	int i,len;
	ut32 data;
	bin->signature = NULL;
	struct linkedit_data_command link = {0};
	ut8 lit[sizeof (struct linkedit_data_command)] = {0};
	struct blob_index_t idx = {0};
	struct super_blob_t super = {{0}};

	if (off > bin->size || off + sizeof (struct linkedit_data_command) > bin->size) {
		return false;
	}
	len = r_buf_read_at (bin->b, off, lit, sizeof (struct linkedit_data_command));
	if (len != sizeof (struct linkedit_data_command)) {
		bprintf ("Failed to get data while parsing LC_CODE_SIGNATURE command\n");
		return false;
	}
	link.cmd = r_read_ble32 (&lit[0], bin->big_endian);
	link.cmdsize = r_read_ble32 (&lit[4], bin->big_endian);
	link.dataoff = r_read_ble32 (&lit[8], bin->big_endian);
	link.datasize = r_read_ble32 (&lit[12], bin->big_endian);

	data = link.dataoff;
	if (data > bin->size || data + sizeof (struct super_blob_t) > bin->size) {
		bin->signature = (ut8 *)strdup ("Malformed entitlement");
		return true;
	}
	super.blob.magic = r_read_ble32 (bin->b->buf + data, little_);
	super.blob.length = r_read_ble32 (bin->b->buf + data + 4, little_);
	super.count = r_read_ble32 (bin->b->buf + data + 8, little_);
	for (i = 0; i < super.count; ++i) {
		if ((ut8 *)(bin->b->buf + data + i) > (ut8 *)(bin->b->buf + bin->size)) {
			bin->signature = (ut8 *)strdup ("Malformed entitlement");
			break;
		}
		struct blob_index_t bi;
		if (r_buf_read_at (bin->b, data + 12 + (i * sizeof (struct blob_index_t)),
			(ut8*)&bi, sizeof (struct blob_index_t)) < sizeof (struct blob_index_t)) {
			break;
		}
		idx.type = r_read_ble32 (&bi.type, little_);
		idx.offset = r_read_ble32 (&bi.offset, little_);
		if (idx.type == CSSLOT_ENTITLEMENTS) {
			ut64 off = data + idx.offset;
			if (off > bin->size || off + sizeof (struct blob_t) > bin->size) {
				bin->signature = (ut8 *)strdup ("Malformed entitlement");
				break;
			}
			struct blob_t entitlements = {0};
			entitlements.magic = r_read_ble32 (bin->b->buf + off, little_);
			entitlements.length = r_read_ble32 (bin->b->buf + off + 4, little_);
			len = entitlements.length - sizeof (struct blob_t);
			if (len <= bin->size && len > 1) {
				bin->signature = calloc (1, len + 1);
				if (bin->signature) {
					ut8 *src = bin->b->buf + off + sizeof (struct blob_t);
					if (off + sizeof (struct blob_t) + len < bin->b->length) {
						memcpy (bin->signature, src, len);
						bin->signature[len] = '\0';
						return true;
					}
					bin->signature = (ut8 *)strdup ("Malformed entitlement");
					return true;
				}
			} else {
				bin->signature = (ut8 *)strdup ("Malformed entitlement");
			}
		}
	}
	if (!bin->signature) {
		bin->signature = (ut8 *)strdup ("No entitlement found");
	}
	return true;
}
