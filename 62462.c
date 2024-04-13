is_32bit_abs_reloc(unsigned int type, Dwarf_Half machine)
{
    Dwarf_Bool r = 0;
    switch (machine) {
#if defined(EM_MIPS) && defined (R_MIPS_32)
    case EM_MIPS:
        r =  (0
#if defined (R_MIPS_32)
            | (type == R_MIPS_32)
#endif
#if defined (R_MIPS_TLS_DTPREL32)
            | (type == R_MIPS_TLS_DTPREL32)
#endif /* DTPREL32 */
            );
        break;
#endif /* MIPS case */
#if defined(EM_SPARC32PLUS)  && defined (R_SPARC_UA32)
    case EM_SPARC32PLUS:
        r =  (type == R_SPARC_UA32);
        break;
#endif
#if defined(EM_SPARCV9)  && defined (R_SPARC_UA32)
    case EM_SPARCV9:
        r =  (type == R_SPARC_UA32);
        break;
#endif
#if defined(EM_SPARC) && defined (R_SPARC_UA32)
    case EM_SPARC:
        r =  (0
#if defined(R_SPARC_UA32)
            | (type == R_SPARC_UA32)
#endif
#if (R_SPARC_TLS_DTPOFF32)
            | (type == R_SPARC_TLS_DTPOFF32)
#endif
            );
        break;
#endif /* EM_SPARC */
#if defined(EM_386) && defined (R_386_32)
    case EM_386:
        r = (0
#if defined (R_386_32)
            |  (type == R_386_32)
#endif
#if defined (R_386_TLS_LDO_32)
            | (type == R_386_TLS_LDO_32)
#endif
#if defined (R_386_TLS_DTPOFF32)
            | (type == R_386_TLS_DTPOFF32)
#endif
            );
        break;
#endif /* EM_386 */

#if defined (EM_SH) && defined (R_SH_DIR32)
    case EM_SH:
        r = (0
#if defined (R_SH_DIR32)
            | (type == R_SH_DIR32)
#endif
#if defined (R_SH_DTPOFF32)
            | (type == R_SH_TLS_DTPOFF32)
#endif
            );
        break;
#endif /* SH */

#if defined(EM_IA_64) && defined (R_IA64_SECREL32LSB)
    case EM_IA_64:  /* 32bit? ! */
        r = (0
#if defined (R_IA64_SECREL32LSB)
            | (type == R_IA64_SECREL32LSB)
#endif
#if defined (R_IA64_DIR32LSB)
            | (type == R_IA64_DIR32LSB)
#endif
#if defined (R_IA64_DTPREL32LSB)
            | (type == R_IA64_DTPREL32LSB)
#endif
            );
        break;
#endif /* EM_IA_64 */

#if defined(EM_ARM) && defined (R_ARM_ABS32)
    case EM_ARM:
    case EM_AARCH64:
        r = (0
#if defined (R_ARM_ABS32)
            | ( type == R_ARM_ABS32)
#endif
#if defined (R_AARCH64_ABS32)
            | ( type == R_AARCH64_ABS32)
#endif
#if defined (R_ARM_TLS_LDO32)
            | ( type == R_ARM_TLS_LDO32)
#endif
            );
        break;
#endif /* EM_ARM */

/*  On FreeBSD R_PPC64_ADDR32 not defined
    so we use the R_PPC_ names which
    have the proper value.
    Our headers have:
    R_PPC64_ADDR64   38
    R_PPC_ADDR32     1 so we use this one
    R_PPC64_ADDR32   R_PPC_ADDR32

    R_PPC64_DTPREL32 110  which may be wrong/unavailable
    R_PPC64_DTPREL64 78
    R_PPC_DTPREL32   78
    */
#if defined(EM_PPC64) && defined (R_PPC_ADDR32)
    case EM_PPC64:
        r = (0
#if defined(R_PPC_ADDR32)
            | (type == R_PPC_ADDR32)
#endif
#if defined(R_PPC64_DTPREL32)
            | (type == R_PPC64_DTPREL32)
#endif
            );
        break;
#endif /* EM_PPC64 */


#if defined(EM_PPC) && defined (R_PPC_ADDR32)
    case EM_PPC:
        r = (0
#if defined (R_PPC_ADDR32)
            | (type == R_PPC_ADDR32)
#endif
#if defined (R_PPC_DTPREL32)
            | (type == R_PPC_DTPREL32)
#endif
            );
        break;
#endif /* EM_PPC */

#if defined(EM_S390) && defined (R_390_32)
    case EM_S390:
        r = (0
#if defined (R_390_32)
            | (type == R_390_32)
#endif
#if defined (R_390_TLS_LDO32)
            | (type == R_390_TLS_LDO32)
#endif
            );
        break;
#endif /* EM_S390 */

#if defined(EM_X86_64) && defined (R_X86_64_32)
#if defined(EM_K10M)
    case EM_K10M:
#endif
#if defined(EM_L10M)
    case EM_L10M:
#endif
    case EM_X86_64:
        r = (0
#if defined (R_X86_64_32)
            | (type == R_X86_64_32)
#endif
#if defined (R_X86_64_DTPOFF32)
            | (type ==  R_X86_64_DTPOFF32)
#endif
            );
        break;
#endif /* EM_X86_64 */

    case  EM_QUALCOMM_DSP6:
        r = (type == QUALCOMM_REL32);
        break;
    }
    return r;
}
