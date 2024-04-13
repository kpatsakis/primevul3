find_section_to_relocate(Dwarf_Debug dbg,Dwarf_Half section_index,
   struct Dwarf_Section_s **relocatablesec, int *error)
{
    MATCH_REL_SEC(section_index,dbg->de_debug_info,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_abbrev,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_line,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_loc,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_aranges,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_macinfo,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_pubnames,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_ranges,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_frame,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_frame_eh_gnu,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_pubtypes,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_funcnames,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_typenames,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_varnames,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_weaknames,relocatablesec);
    MATCH_REL_SEC(section_index,dbg->de_debug_types,relocatablesec);
    /* dbg-> de_debug_tu_index,reloctablesec); */
    /* dbg-> de_debug_cu_index,reloctablesec); */
    /* dbg-> de_debug_gdbindex,reloctablesec); */
    /* dbg-> de_debug_str,syms); */
    /* de_elf_symtab,syms); */
    /* de_elf_strtab,syms); */
    *error = DW_DLE_RELOC_SECTION_MISMATCH;
    return DW_DLV_ERROR;

}
