get_rela_elf32(Dwarf_Small *data, unsigned int i,
  int endianness,
  int machine,
  struct Dwarf_Elf_Rela *relap)
{
    Elf32_Rela *relp = (Elf32_Rela*)(data + (i * sizeof(Elf32_Rela)));
    relap->r_offset = relp->r_offset;
    /*
    relap->r_info = relp->r_info;
   */
    relap->r_type = ELF32_R_TYPE(relp->r_info);
    relap->r_symidx = ELF32_R_SYM(relp->r_info);
    relap->r_addend = relp->r_addend;
}
