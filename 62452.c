dwarf_elf_object_access_get_pointer_size(void* obj_in)
{
    dwarf_elf_object_access_internals_t*obj =
        (dwarf_elf_object_access_internals_t*)obj_in;
    return obj->pointer_size;
}
