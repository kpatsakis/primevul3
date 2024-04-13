unsigned int jsvGetMemoryUsage() {
  unsigned int usage = 0;
  unsigned int i;
  for (i=1;i<=jsVarsSize;i++) {
    JsVar *v = jsvGetAddressOf((JsVarRef)i);
    if ((v->flags&JSV_VARTYPEMASK) != JSV_UNUSED) {
      usage++;
      if (jsvIsFlatString(v)) {
        unsigned int b = (unsigned int)jsvGetFlatStringBlocks(v);
        i+=b;
        usage+=b;
      }
    }
  }
  return usage;
}
