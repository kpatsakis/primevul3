void jsvClearEmptyVarList() {
  assert(!isMemoryBusy);
  isMemoryBusy = MEMBUSY_SYSTEM;
  jsVarFirstEmpty = 0;
  JsVarRef i;
  for (i=1;i<=jsVarsSize;i++) {
    JsVar *var = jsvGetAddressOf(i);
    if ((var->flags&JSV_VARTYPEMASK) == JSV_UNUSED) {
      memset((void*)var,0,sizeof(JsVar));
    } else if (jsvIsFlatString(var)) {
      i = (JsVarRef)(i+jsvGetFlatStringBlocks(var));
    }
  }
  isMemoryBusy = MEM_NOT_BUSY;
}
