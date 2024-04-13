void jsvCreateEmptyVarList() {
  assert(!isMemoryBusy);
  isMemoryBusy = MEMBUSY_SYSTEM;
  jsVarFirstEmpty = 0;
  JsVar firstVar; // temporary var to simplify code in the loop below
  jsvSetNextSibling(&firstVar, 0);
  JsVar *lastEmpty = &firstVar;

  JsVarRef i;
  for (i=1;i<=jsVarsSize;i++) {
    JsVar *var = jsvGetAddressOf(i);
    if ((var->flags&JSV_VARTYPEMASK) == JSV_UNUSED) {
      jsvSetNextSibling(lastEmpty, i);
      lastEmpty = var;
    } else if (jsvIsFlatString(var)) {
      i = (JsVarRef)(i+jsvGetFlatStringBlocks(var));
    }
  }
  jsvSetNextSibling(lastEmpty, 0);
  jsVarFirstEmpty = jsvGetNextSibling(&firstVar);
  isMemoryBusy = MEM_NOT_BUSY;
}
