is_64bit_abs_reloc(unsigned int type, Dwarf_Half machine)
{
    Dwarf_Bool r = 0;
    switch (machine) {
#if defined(EM_MIPS) && defined (R_MIPS_64)
    case EM_MIPS:
        r = (0
#if defined (R_MIPS_64)
            | (type == R_MIPS_64)
#endif
#if defined (R_MIPS_32)
            | (type == R_MIPS_32)
#endif
#if defined(R_MIPS_TLS_DTPREL64)
            | (type == R_MIPS_TLS_DTPREL64)
#endif
            );
        break;
#endif /* EM_MIPS */
#if defined(EM_SPARC32PLUS) && defined (R_SPARC_UA64)
    case EM_SPARC32PLUS:
        r =  (type == R_SPARC_UA64);
        break;
#endif
#if defined(EM_SPARCV9) && defined (R_SPARC_UA64)
    case EM_SPARCV9:
        r = (0
#if defined (R_SPARC_UA64)
            | (type == R_SPARC_UA64)
#endif
#if defined (R_SPARC_TLS_DTPOFF64)
            | (type == R_SPARC_TLS_DTPOFF64)
#endif
            );
        break;
#endif
#if defined(EM_SPARC) && defined (R_SPARC_UA64)
    case EM_SPARC:
        r = (0
#if defined(R_SPARC_UA64)
            | (type == R_SPARC_UA64)
#endif
#if defined (R_SPARC_TLS_DTPOFF64)
            | (type == R_SPARC_TLS_DTPOFF64)
#endif
            );
        break;
#endif /* EM_SPARC */

#if defined(EM_IA_64) && defined (R_IA64_SECREL64LSB)
    case EM_IA_64: /* 64bit */
        r = (0
#if defined (R_IA64_SECREL64LSB)
            | (type == R_IA64_SECREL64LSB)
#endif
#if defined (R_IA64_SECREL32LSB)
            | (type == R_IA64_SECREL32LSB)
#endif
#if defined (R_IA64_DIR64LSB)
            | (type == R_IA64_DIR64LSB)
#endif
#if defined (R_IA64_DTPREL64LSB)
            | (type == R_IA64_DTPREL64LSB)
#endif
#if defined (R_IA64_REL32LSB)
            | (type == R_IA64_REL32LSB)
#endif
            );
        break;
#endif /* EM_IA_64 */

#if defined(EM_PPC64) && defined (R_PPC64_ADDR64)
    case EM_PPC64:
        r = (0
#if defined(R_PPC64_ADDR64)
            | (type == R_PPC64_ADDR64)
#endif
#if defined(R_PPC64_DTPREL64)
            | (type == R_PPC64_DTPREL64)
#endif
            );
        break;
#endif /* EM_PPC64 */

#if defined(EM_S390) && defined (R_390_64)
    case EM_S390:
        r = (0
#if defined(R_390_64)
            | (type == R_390_64)
#endif
#if defined(R_390_TLS_LDO64)
            | (type == R_390_TLS_LDO64)
#endif
            );
        break;
#endif /* EM_390 */

#if defined(EM_X86_64) && defined (R_X86_64_64)
#if defined(EM_K10M)
    case EM_K10M:
#endif
#if defined(EM_L10M)
    case EM_L10M:
#endif
    case EM_X86_64:
        r = (0
#if defined (R_X86_64_64)
            | (type == R_X86_64_64)
#endif
#if defined (R_X86_64_DTPOFF32)
            | (type == R_X86_64_DTPOFF64)
#endif
            );
        break;
#endif /* EM_X86_64 */
#if defined(EM_AARCH64) && defined (R_AARCH64_ABS64)
    case EM_AARCH64:
        r = (0
#if defined (R_AARCH64_ABS64)
            | ( type == R_AARCH64_ABS64)
#endif
            );
        break;
#endif /* EM_AARCH64 */

    }
    return r;
}
