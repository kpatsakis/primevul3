int jsvGarbageCollect() {
  if (isMemoryBusy) return false;
  isMemoryBusy = MEMBUSY_GC;
  JsVarRef i;
  for (i=1;i<=jsVarsSize;i++)  {
    JsVar *var = jsvGetAddressOf(i);
    if ((var->flags&JSV_VARTYPEMASK) != JSV_UNUSED) { // if it is not unused
      var->flags |= (JsVarFlags)JSV_GARBAGE_COLLECT;
      if (jsvIsFlatString(var))
        i = (JsVarRef)(i+jsvGetFlatStringBlocks(var));
    }
  }
  /* recursively remove anything that is referenced from a var that is locked. */
  for (i=1;i<=jsVarsSize;i++)  {
    JsVar *var = jsvGetAddressOf(i);
    if ((var->flags & JSV_GARBAGE_COLLECT) && // not already GC'd
        jsvGetLocks(var)>0) // or it is locked
      jsvGarbageCollectMarkUsed(var);
    if (jsvIsFlatString(var))
      i = (JsVarRef)(i+jsvGetFlatStringBlocks(var));
  }
  /* now sweep for things that we can GC!
   * Also update the free list - this means that every new variable that
   * gets allocated gets allocated towards the start of memory, which
   * hopefully helps compact everything towards the start. */
  unsigned int freedCount = 0;
  jsVarFirstEmpty = 0;
  JsVar *lastEmpty = 0;
  for (i=1;i<=jsVarsSize;i++)  {
    JsVar *var = jsvGetAddressOf(i);
    if (var->flags & JSV_GARBAGE_COLLECT) {
      if (jsvIsFlatString(var)) {
        unsigned int count = (unsigned int)jsvGetFlatStringBlocks(var);
        freedCount+=count;
        var->flags = JSV_UNUSED;
        if (lastEmpty) jsvSetNextSibling(lastEmpty, i);
        else jsVarFirstEmpty = i;
        lastEmpty = var;
        while (count-- > 0) {
          i++;
          var = jsvGetAddressOf((JsVarRef)(i));
          var->flags = JSV_UNUSED;
          if (lastEmpty) jsvSetNextSibling(lastEmpty, i);
          else jsVarFirstEmpty = i;
          lastEmpty = var;
        }
      } else {
        if (jsvHasSingleChild(var)) {
          /* If this had a child that wasn't listed for GC then we need to
           * unref it. Everything else is fine because it'll disappear anyway.
           * We don't have to check if we should free this other variable
           * here because we know the GC picked up it was referenced from
           * somewhere else. */
          JsVarRef ch = jsvGetFirstChild(var);
          if (ch) {
            JsVar *child = jsvGetAddressOf(ch); // not locked
            if (child->flags!=JSV_UNUSED && // not already GC'd!
                !(child->flags&JSV_GARBAGE_COLLECT)) // not marked for GC
              jsvUnRef(child);
          }
        }
        /* Sanity checks here. We're making sure that any variables that are
         * linked from this one have either already been garbage collected or
         * are marked for GC */
        assert(!jsvHasChildren(var) || !jsvGetFirstChild(var) ||
            jsvGetAddressOf(jsvGetFirstChild(var))->flags==JSV_UNUSED ||
            (jsvGetAddressOf(jsvGetFirstChild(var))->flags&JSV_GARBAGE_COLLECT));
        assert(!jsvHasChildren(var) || !jsvGetLastChild(var) ||
            jsvGetAddressOf(jsvGetLastChild(var))->flags==JSV_UNUSED ||
            (jsvGetAddressOf(jsvGetLastChild(var))->flags&JSV_GARBAGE_COLLECT));
        assert(!jsvIsName(var) || !jsvGetPrevSibling(var) ||
            jsvGetAddressOf(jsvGetPrevSibling(var))->flags==JSV_UNUSED ||
            (jsvGetAddressOf(jsvGetPrevSibling(var))->flags&JSV_GARBAGE_COLLECT));
        assert(!jsvIsName(var) || !jsvGetNextSibling(var) ||
            jsvGetAddressOf(jsvGetNextSibling(var))->flags==JSV_UNUSED ||
            (jsvGetAddressOf(jsvGetNextSibling(var))->flags&JSV_GARBAGE_COLLECT));
        var->flags = JSV_UNUSED;
        if (lastEmpty) jsvSetNextSibling(lastEmpty, i);
        else jsVarFirstEmpty = i;
        lastEmpty = var;
        freedCount++;
      }
    } else if (jsvIsFlatString(var)) {
      i = (JsVarRef)(i+jsvGetFlatStringBlocks(var));
    } else if (var->flags == JSV_UNUSED) {
      if (lastEmpty) jsvSetNextSibling(lastEmpty, i);
      else jsVarFirstEmpty = i;
      lastEmpty = var;
    }
  }
  if (lastEmpty) jsvSetNextSibling(lastEmpty, 0);
  isMemoryBusy = MEM_NOT_BUSY;
  return (int)freedCount;
}
