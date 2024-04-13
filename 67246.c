static void bin_pe_store_tls_callbacks(struct PE_(r_bin_pe_obj_t)* bin, PE_DWord callbacks) {
	PE_DWord paddr, haddr;
	int count = 0;
	PE_DWord addressOfTLSCallback = 1;
	char* key;

	while (addressOfTLSCallback != 0) {
		if (r_buf_read_at (bin->b, callbacks, (ut8*) &addressOfTLSCallback, sizeof(addressOfTLSCallback)) != sizeof (addressOfTLSCallback)) {
			bprintf ("Warning: read (tls_callback)\n");
			return;
		}
		if (!addressOfTLSCallback) {
			break;
		}
		if (bin->optional_header->SizeOfImage) {
			int rva_callback = bin_pe_va_to_rva (bin, (PE_DWord) addressOfTLSCallback);
			if (rva_callback > bin->optional_header->SizeOfImage) {
				break;
			}
		}
		key = sdb_fmt ("pe.tls_callback%d_vaddr", count);
		sdb_num_set (bin->kv, key, addressOfTLSCallback, 0);
		key = sdb_fmt ("pe.tls_callback%d_paddr", count);
		paddr = bin_pe_rva_to_paddr (bin, bin_pe_va_to_rva (bin, (PE_DWord) addressOfTLSCallback));
		sdb_num_set (bin->kv, key, paddr,                0);
		key = sdb_fmt ("pe.tls_callback%d_haddr", count);
		haddr = callbacks;
		sdb_num_set (bin->kv, key, haddr,                0);
		count++;
		callbacks += sizeof (addressOfTLSCallback);
	}
}
