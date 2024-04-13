update_entry(Dwarf_Debug dbg,
    Dwarf_Bool is_64bit, Dwarf_Endianness endianess,
    Dwarf_Half machine, struct Dwarf_Elf_Rela *rela,
    Dwarf_Small *target_section,
    Dwarf_Small *symtab_section_data,
    Dwarf_Unsigned symtab_section_size,
    Dwarf_Unsigned symtab_section_entrysize,
    int *error)
{
    unsigned int type = 0;
    unsigned int sym_idx = 0;
#ifdef HAVE_ELF64_SYM
    Elf64_Sym sym_buf;
    Elf64_Sym *sym = 0;
#else
    Elf32_Sym sym_buf;
    Elf32_Sym *sym = 0;
#endif
    Elf32_Sym *sym32 = 0;
    Dwarf_ufixed64 offset = 0;
    Dwarf_sfixed64 addend = 0;
    Dwarf_Unsigned reloc_size = 0;
    Dwarf_Unsigned symtab_entry_count = 0;

    if (symtab_section_entrysize == 0) {
        *error = DW_DLE_SYMTAB_SECTION_ENTRYSIZE_ZERO;
        return DW_DLV_ERROR;
    }
    symtab_entry_count = symtab_section_size/symtab_section_entrysize;

    /* Dwarf_Elf_Rela dereferencing */
    offset = rela->r_offset;
    addend = rela->r_addend;
    type = rela->r_type;
    sym_idx = rela->r_symidx;
    if (sym_idx >= symtab_entry_count) {
        *error = DW_DLE_RELOC_SECTION_SYMBOL_INDEX_BAD;
        return DW_DLV_ERROR;
    }



    if (is_64bit) {
#ifdef HAVE_ELF64_SYM
        sym = &((Elf64_Sym*)symtab_section_data)[sym_idx];
#endif
    } else {
        sym32 = &((Elf32_Sym*)symtab_section_data)[sym_idx];

        /*  Convert Elf32_Sym struct to Elf64_Sym struct. We point at
            an Elf64_Sym local variable (sym_buf) to allow us to use the
            same pointer (sym) for both 32-bit and 64-bit instances.  */
        sym = &sym_buf;
        sym->st_name = sym32->st_name;
        sym->st_info = sym32->st_info;
        sym->st_other = sym32->st_other;
        sym->st_shndx = sym32->st_shndx;
        sym->st_value = sym32->st_value;
        sym->st_size = sym32->st_size;
    }

    /* Determine relocation size */
    if (is_32bit_abs_reloc(type, machine)) {
        reloc_size = 4;
    } else if (is_64bit_abs_reloc(type, machine)) {
        reloc_size = 8;
    } else {
        *error = DW_DLE_RELOC_SECTION_RELOC_TARGET_SIZE_UNKNOWN;
        return DW_DLV_ERROR;
    }


    {
        /*  Assuming we do not need to do a READ_UNALIGNED here
            at target_section + offset and add its value to
            outval.  Some ABIs say no read (for example MIPS),
            but if some do then which ones? */
        Dwarf_Unsigned outval = sym->st_value + addend;
        WRITE_UNALIGNED(dbg,target_section + offset,
            &outval,sizeof(outval),reloc_size);
    }
    return DW_DLV_OK;
}
