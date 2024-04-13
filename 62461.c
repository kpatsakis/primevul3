get_relocations_array(Dwarf_Bool is_64bit,
    int endianness,
    int machine,
    Dwarf_Small *data,
    unsigned int num_relocations,
    struct Dwarf_Elf_Rela *relap)
{
    unsigned int i = 0;
    void (*get_relocations)(Dwarf_Small *data, unsigned int i,
        int endianness,
        int machine,
        struct Dwarf_Elf_Rela *relap);

    /* Handle 32/64 bit issue */
    if (is_64bit) {
        get_relocations = get_rela_elf64;
    } else {
        get_relocations = get_rela_elf32;
    }

    for (i=0; i < num_relocations; i++) {
        get_relocations(data, i,endianness,machine,
            &(relap[i]));
    }

}
