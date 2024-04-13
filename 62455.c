dwarf_elf_object_access_init(dwarf_elf_handle elf,
    int libdwarf_owns_elf,
    Dwarf_Obj_Access_Interface** ret_obj,
    int *err)
{
    int res = 0;
    dwarf_elf_object_access_internals_t *internals = 0;
    Dwarf_Obj_Access_Interface *intfc = 0;

    internals = malloc(sizeof(dwarf_elf_object_access_internals_t));
    if (!internals) {
        *err = DW_DLE_ALLOC_FAIL;
        /* Impossible case, we hope. Give up. */
        return DW_DLV_ERROR;
    }
    memset(internals,0,sizeof(*internals));
    res = dwarf_elf_object_access_internals_init(internals, elf, err);
    if (res != DW_DLV_OK){
        /* *err is already set. */
        free(internals);
        return DW_DLV_ERROR;
    }
    internals->libdwarf_owns_elf = libdwarf_owns_elf;

    intfc = malloc(sizeof(Dwarf_Obj_Access_Interface));
    if (!intfc) {
        /* Impossible case, we hope. Give up. */
        *err = DW_DLE_ALLOC_FAIL;
        free(internals);
        return DW_DLV_ERROR;
    }
    /* Initialize the interface struct */
    intfc->object = internals;
    intfc->methods = &dwarf_elf_object_access_methods;

    *ret_obj = intfc;
    return DW_DLV_OK;
}
