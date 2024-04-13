ALWAYS_INLINE JsVarRef jsvGetRef(JsVar *var) {
  if (!var) return 0;
#ifdef RESIZABLE_JSVARS
  unsigned int i, c = jsVarsSize>>JSVAR_BLOCK_SHIFT;
  for (i=0;i<c;i++) {
    if (var>=jsVarBlocks[i] && var<&jsVarBlocks[i][JSVAR_BLOCK_SIZE]) {
      JsVarRef r = (JsVarRef)(1 + (i<<JSVAR_BLOCK_SHIFT) + (var - jsVarBlocks[i]));
      return r;
    }
  }
  return 0;
#else
  return (JsVarRef)(1 + (var - jsVars));
#endif
}
