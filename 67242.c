static int bin_pe_init_sections(struct PE_(r_bin_pe_obj_t)* bin) {
	bin->num_sections = bin->nt_headers->file_header.NumberOfSections;
	int sections_size;
	if (bin->num_sections < 1) {
		return true;
	}
	sections_size = sizeof (PE_(image_section_header)) * bin->num_sections;
	if (sections_size > bin->size) {
		sections_size = bin->size;
		bin->num_sections = bin->size / sizeof (PE_(image_section_header));
	}
	if (!(bin->section_header = malloc (sections_size))) {
		r_sys_perror ("malloc (section header)");
		goto out_error;
	}
	bin->section_header_offset = bin->dos_header->e_lfanew + 4 + sizeof (PE_(image_file_header)) +
		bin->nt_headers->file_header.SizeOfOptionalHeader;
	if (r_buf_read_at (bin->b, bin->section_header_offset,
		(ut8*) bin->section_header, sections_size) == -1) {
		bprintf ("Warning: read (sections)\n");
		R_FREE (bin->section_header);
		goto out_error;
	}
#if 0
	Each symbol table entry includes a name, storage class, type, value and section number.Short names (8 characters or fewer) are stored directly in the symbol table;
	longer names are stored as an paddr into the string table at the end of the COFF object.

	================================================================
	COFF SYMBOL TABLE RECORDS (18 BYTES)
	================================================================
	record
	paddr

	struct symrec {
		union {
			char string[8]; // short name
			struct {
				ut32 seros;
				ut32 stridx;
			} stridx;
		} name;
		ut32 value;
		ut16 secnum;
		ut16 symtype;
		ut8 symclass;
		ut8 numaux;
	}
	------------------------------------------------------ -
	0 | 8 - char symbol name |
	| or 32 - bit zeroes followed by 32 - bit |
	| index into string table |
	------------------------------------------------------ -
	8 | symbol value |
	------------------------------------------------------ -
	0Ch | section number | symbol type |
	------------------------------------------------------ -
	10h | sym class | num aux |
	-------------------------- -
	12h

#endif
	return true;
out_error:
	bin->num_sections = 0;
	return false;
}
