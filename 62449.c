dwarf_elf_object_access_finish(Dwarf_Obj_Access_Interface* obj)
{
    if (!obj) {
        return;
    }
    if (obj->object) {
        dwarf_elf_object_access_internals_t *internals =
            (dwarf_elf_object_access_internals_t *)obj->object;
        if (internals->libdwarf_owns_elf){
            elf_end(internals->elf);
        }
    }
    free(obj->object);
    free(obj);
}
