JsVar *jsvFindOrCreateRoot() {
  JsVarRef i;
  for (i=1;i<=jsVarsSize;i++)
    if (jsvIsRoot(jsvGetAddressOf(i)))
      return jsvLock(i);

  return jsvRef(jsvNewWithFlags(JSV_ROOT));
}
