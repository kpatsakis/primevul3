get_rela_elf64(Dwarf_Small *data, unsigned int i,
  int endianness,
  int machine,
  struct Dwarf_Elf_Rela *relap)
{
#ifdef HAVE_ELF64_RELA
    Elf64_Rela * relp = (Elf64_Rela*)(data + (i * sizeof(Elf64_Rela)));
    relap->r_offset = relp->r_offset;
    /*
    relap->r_info = relp->r_info;
    */
#define ELF64MIPS_REL_SYM(i) ((i) & 0xffffffff)
#define ELF64MIPS_REL_TYPE(i) ((i >> 56) &0xff)
    if (machine == EM_MIPS && endianness == DW_OBJECT_LSB ){
        /*  This is really wierd. Treat this very specially.
            The Elf64 LE MIPS object used for
            testing (that has rela) wants the
            values as  sym  ssym type3 type2 type, treating
            each value as independent value. But libelf xlate
            treats it as something else so we fudge here.
            It is unclear
            how to precisely characterize where these relocations
            were used.
            SGI MIPS on IRIX never used .rela relocations.
            The BE 64bit elf MIPS test object with rela uses traditional
            elf relocation layouts, not this special case.  */
        /*  We ignore the special TYPE2 and TYPE3, they should be
            value R_MIPS_NONE in rela. */
        relap->r_type = ELF64MIPS_REL_TYPE(relp->r_info);
        relap->r_symidx = ELF64MIPS_REL_SYM(relp->r_info);
#undef MIPS64SYM
#undef MIPS64TYPE
    } else
    {
        relap->r_type = ELF64_R_TYPE(relp->r_info);
        relap->r_symidx = ELF64_R_SYM(relp->r_info);
    }
    relap->r_addend = relp->r_addend;
#endif
}
