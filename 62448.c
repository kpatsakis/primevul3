apply_rela_entries(Dwarf_Debug dbg,
    Dwarf_Bool is_64bit,
    Dwarf_Endianness endianess,
    Dwarf_Half machine,
    Dwarf_Small *target_section,
    Dwarf_Small *symtab_section,
    Dwarf_Unsigned symtab_section_size,
    Dwarf_Unsigned symtab_section_entrysize,
    struct Dwarf_Elf_Rela *relas, unsigned int nrelas,
    int *error)
{
    int return_res = DW_DLV_OK;
    if ((target_section != NULL)  && (relas != NULL)) {
        unsigned int i;
        if (symtab_section_entrysize == 0) {
            *error = DW_DLE_SYMTAB_SECTION_ENTRYSIZE_ZERO;
            return DW_DLV_ERROR;
        }
        if (symtab_section_size%symtab_section_entrysize) {
            *error = DW_DLE_SYMTAB_SECTION_LENGTH_ODD;
            return DW_DLV_ERROR;
        }
        for (i = 0; i < nrelas; i++) {
            int res = update_entry(dbg, is_64bit,
                endianess,
                machine,
                &(relas)[i],
                target_section,
                symtab_section,
                symtab_section_size,
                symtab_section_entrysize,
                error);
            if (res != DW_DLV_OK) {
                return_res = res;
            }
        }
    }
    return return_res;
}
