get_relocation_entries(Dwarf_Bool is_64bit,
    int endianness,
    int machine,
    Dwarf_Small *relocation_section,
    Dwarf_Unsigned relocation_section_size,
    Dwarf_Unsigned relocation_section_entrysize,
    struct Dwarf_Elf_Rela **relas,
    unsigned int *nrelas,
    int *error)
{
    unsigned int relocation_size = 0;

    if (is_64bit) {
#ifdef HAVE_ELF64_RELA
        relocation_size = sizeof(Elf64_Rela);
#else
        *error = DW_DLE_MISSING_ELF64_SUPPORT;
        return DW_DLV_ERROR;
#endif
    } else {
        relocation_size = sizeof(Elf32_Rela);
    }
    if (relocation_size != relocation_section_entrysize) {
        /*  Means our struct definition does not match the
            real object. */
        *error = DW_DLE_RELOC_SECTION_LENGTH_ODD;
        return DW_DLV_ERROR;
    }

    if (relocation_section == NULL) {
        *error = DW_DLE_RELOC_SECTION_PTR_NULL;
        return(DW_DLV_ERROR);
    }

    if ((relocation_section_size != 0)) {
        size_t bytescount = 0;
        if (relocation_section_size%relocation_size) {
            *error = DW_DLE_RELOC_SECTION_LENGTH_ODD;
            return DW_DLV_ERROR;
        }
        *nrelas = relocation_section_size/relocation_size;
        bytescount = (*nrelas) * sizeof(struct Dwarf_Elf_Rela);
        *relas = malloc(bytescount);
        if (!*relas) {
            *error = DW_DLE_MAF;
            return(DW_DLV_ERROR);
        }
        memset(*relas,0,bytescount);
        get_relocations_array(is_64bit,endianness,machine,
            relocation_section,
            *nrelas, *relas);
    }
    return(DW_DLV_OK);
}
