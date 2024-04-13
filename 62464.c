loop_through_relocations(
   Dwarf_Debug dbg,
   dwarf_elf_object_access_internals_t* obj,
   struct Dwarf_Section_s *relocatablesec,
   int *error)
{
    Dwarf_Small *target_section = 0;
    Dwarf_Small *symtab_section = obj->symtab->dss_data;
    Dwarf_Unsigned symtab_section_entrysize = obj->symtab->dss_entrysize;
    Dwarf_Unsigned symtab_section_size = obj->symtab->dss_size;
    Dwarf_Small *relocation_section  = relocatablesec->dss_reloc_data;
    Dwarf_Unsigned relocation_section_size =
        relocatablesec->dss_reloc_size;
    Dwarf_Unsigned relocation_section_entrysize = relocatablesec->dss_reloc_entrysize;

    int ret = DW_DLV_ERROR;
    struct Dwarf_Elf_Rela *relas = 0;
    unsigned int nrelas = 0;
    Dwarf_Small *mspace = 0;

    ret = get_relocation_entries(obj->is_64bit,
        obj->endianness,
        obj->machine,
        relocation_section,
        relocation_section_size,
        relocation_section_entrysize,
        &relas, &nrelas, error);
    if (ret != DW_DLV_OK) {
        free(relas);
        return ret;
    }

    if(!relocatablesec->dss_data_was_malloc) {
        /*  Some systems read Elf in read-only memory via mmap or the like.
            So the only safe thing is to copy the current data into
            malloc space and refer to the malloc space instead of the
            space returned by the elf library */
        mspace = malloc(relocatablesec->dss_size);
        if (!mspace) {
            *error = DW_DLE_RELOC_SECTION_MALLOC_FAIL;
            return DW_DLV_ERROR;
        }
        memcpy(mspace,relocatablesec->dss_data,relocatablesec->dss_size);
        relocatablesec->dss_data = mspace;
        target_section = relocatablesec->dss_data;
        relocatablesec->dss_data_was_malloc = 1;
    }
    target_section = relocatablesec->dss_data;

    ret = apply_rela_entries(
        dbg,
        obj->is_64bit,
        obj->endianness, obj->machine,
        target_section,
        symtab_section,
        symtab_section_size,
        symtab_section_entrysize,
        relas, nrelas, error);
    free(relas);
    return ret;
}
