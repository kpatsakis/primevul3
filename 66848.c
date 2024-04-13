void jsvDumpLockedVars() {
  jsvGarbageCollect();
  if (isMemoryBusy) return;
  isMemoryBusy = MEMBUSY_SYSTEM;
  JsVarRef i;
  for (i=1;i<=jsVarsSize;i++)  {
    JsVar *var = jsvGetAddressOf(i);
    if ((var->flags&JSV_VARTYPEMASK) != JSV_UNUSED) { // if it is not unused
      var->flags |= (JsVarFlags)JSV_GARBAGE_COLLECT;
      if (jsvIsFlatString(var))
        i = (JsVarRef)(i+jsvGetFlatStringBlocks(var));
    }
  }
  jsvGarbageCollectMarkUsed(execInfo.root);
  for (i=1;i<=jsVarsSize;i++)  {
    JsVar *var = jsvGetAddressOf(i);
    if ((var->flags&JSV_VARTYPEMASK) != JSV_UNUSED) {
      if (var->flags & JSV_GARBAGE_COLLECT) {
        jsvGarbageCollectMarkUsed(var);
        jsvTrace(var, 0);
      }
    }
  }
  isMemoryBusy = MEM_NOT_BUSY;
}
