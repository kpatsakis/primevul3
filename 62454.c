dwarf_elf_object_access_get_section_info(
    void* obj_in,
    Dwarf_Half section_index,
    Dwarf_Obj_Access_Section* ret_scn,
    int* error)
{
    dwarf_elf_object_access_internals_t*obj =
        (dwarf_elf_object_access_internals_t*)obj_in;

    Elf32_Shdr *shdr32 = 0;

#ifdef HAVE_ELF64_GETSHDR
    Elf64_Shdr *shdr64 = 0;
#endif
    Elf_Scn *scn = 0;


    scn = elf_getscn(obj->elf, section_index);
    if (scn == NULL) {
        *error = DW_DLE_MDE;
        return DW_DLV_ERROR;
    }
    if (obj->is_64bit) {
#ifdef HAVE_ELF64_GETSHDR
        shdr64 = elf64_getshdr(scn);
        if (shdr64 == NULL) {
            *error = DW_DLE_ELF_GETSHDR_ERROR;
            return DW_DLV_ERROR;
        }

        /*  Get also section 'sh_type' and sh_info' fields, so the caller
            can use it for additional tasks that require that info. */
        ret_scn->type = shdr64->sh_type;
        ret_scn->size = shdr64->sh_size;
        ret_scn->addr = shdr64->sh_addr;
        ret_scn->link = shdr64->sh_link;
        ret_scn->info = shdr64->sh_info;
        ret_scn->entrysize = shdr64->sh_entsize;
        ret_scn->name = elf_strptr(obj->elf, obj->ehdr64->e_shstrndx,
            shdr64->sh_name);
        if (ret_scn->name == NULL) {
            *error = DW_DLE_ELF_STRPTR_ERROR;
            return DW_DLV_ERROR;
        }
        return DW_DLV_OK;
#else
        *error = DW_DLE_MISSING_ELF64_SUPPORT;
        return DW_DLV_ERROR;
#endif /* HAVE_ELF64_GETSHDR */
    }
    if ((shdr32 = elf32_getshdr(scn)) == NULL) {
        *error = DW_DLE_ELF_GETSHDR_ERROR;
        return DW_DLV_ERROR;
    }

    /*  Get also the section type, so the caller can use it for
        additional tasks that require to know the section type. */
    ret_scn->type = shdr32->sh_type;
    ret_scn->size = shdr32->sh_size;
    ret_scn->addr = shdr32->sh_addr;
    ret_scn->link = shdr32->sh_link;
    ret_scn->info = shdr32->sh_info;
    ret_scn->entrysize = shdr32->sh_entsize;
    ret_scn->name = elf_strptr(obj->elf, obj->ehdr32->e_shstrndx,
        shdr32->sh_name);
    if (ret_scn->name == NULL) {
        *error = DW_DLE_ELF_STRPTR_ERROR;
        return DW_DLV_ERROR;
    }
    return DW_DLV_OK;
}
