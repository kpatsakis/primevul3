static ALWAYS_INLINE JsVar *jsvGetAddressOf(JsVarRef ref) {
  assert(ref);
#ifdef RESIZABLE_JSVARS
  JsVarRef t = ref-1;
  return &jsVarBlocks[t>>JSVAR_BLOCK_SHIFT][t&(JSVAR_BLOCK_SIZE-1)];
#else
  return &jsVars[ref-1];
#endif
}
