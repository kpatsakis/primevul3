int PE_(bin_pe_get_overlay)(struct PE_(r_bin_pe_obj_t)* bin, ut64* size) {
	ut64 largest_offset = 0;
	ut64 largest_size = 0;
	*size = 0;
	int i;

	if (!bin) {
		return 0;
	}

	if (bin->optional_header) {
		computeOverlayOffset (
				bin->nt_header_offset+4+sizeof(bin->nt_headers->file_header),
				bin->nt_headers->file_header.SizeOfOptionalHeader,
				bin->size,
				&largest_offset,
				&largest_size);
	}

	struct r_bin_pe_section_t *sects = NULL;
	sects = PE_(r_bin_pe_get_sections) (bin);
	for (i = 0; !sects[i].last; i++) {
		computeOverlayOffset(
				sects[i].paddr,
				sects[i].size,
				bin->size,
				&largest_offset,
				&largest_size
				);
	}

	if (bin->optional_header) {
		for (i = 0; i < PE_IMAGE_DIRECTORY_ENTRIES; i++) {
			if (i == PE_IMAGE_DIRECTORY_ENTRY_SECURITY) {
				continue;
			}

			computeOverlayOffset (
				bin_pe_rva_to_paddr (bin, bin->data_directory[i].VirtualAddress),
				bin->data_directory[i].Size,
				bin->size,
				&largest_offset,
				&largest_size);
		}

	}

	if ((ut64) bin->size > largest_offset + largest_size) {
		*size = bin->size - largest_offset - largest_size;
		free (sects);
		return largest_offset + largest_size;
	}
	free (sects);
	return 0;
}
