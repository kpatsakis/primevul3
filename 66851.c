ALWAYS_INLINE void jsvFreePtr(JsVar *var) {
  /* To be here, we're not supposed to be part of anything else. If
   * we were, we'd have been freed by jsvGarbageCollect */
  assert((!jsvGetNextSibling(var) && !jsvGetPrevSibling(var)) || // check that next/prevSibling are not set
      jsvIsRefUsedForData(var) ||  // UNLESS we're part of a string and nextSibling/prevSibling are used for string data
      (jsvIsName(var) && (jsvGetNextSibling(var)==jsvGetPrevSibling(var)))); // UNLESS we're signalling that we're jsvIsNewChild

  if (jsvIsNameWithValue(var)) {
#ifdef CLEAR_MEMORY_ON_FREE
    jsvSetFirstChild(var, 0); // it just contained random data - zero it
#endif // CLEAR_MEMORY_ON_FREE
  } else if (jsvHasSingleChild(var)) {
    if (jsvGetFirstChild(var)) {
      JsVar *child = jsvLock(jsvGetFirstChild(var));
      jsvUnRef(child);
#ifdef CLEAR_MEMORY_ON_FREE
      jsvSetFirstChild(var, 0); // unlink the child
#endif // CLEAR_MEMORY_ON_FREE
      jsvUnLock(child); // unlock should trigger a free
    }
  }
  /* No else, because a String Name may have a single child, but
   * also StringExts  */

  /* Now, free children - see jsvar.h comments for how! */
  if (jsvHasStringExt(var)) {
    JsVarRef stringDataRef = jsvGetLastChild(var);
#ifdef CLEAR_MEMORY_ON_FREE
    jsvSetLastChild(var, 0);
#endif // CLEAR_MEMORY_ON_FREE
    while (stringDataRef) {
      JsVar *child = jsvGetAddressOf(stringDataRef);
      assert(jsvIsStringExt(child));
      stringDataRef = jsvGetLastChild(child);
      jsvFreePtrInternal(child);
    }
    if (jsvIsFlatString(var)) {
      size_t count = jsvGetFlatStringBlocks(var);
      JsVarRef i = (JsVarRef)(jsvGetRef(var)+count);
      while (count--) {
        JsVar *p = jsvGetAddressOf(i--);
        p->flags = JSV_UNUSED; // set locks to 0 so the assert in jsvFreePtrInternal doesn't get fed up
        jsvFreePtrInternal(p);
      }
    } else if (jsvIsBasicString(var)) {
#ifdef CLEAR_MEMORY_ON_FREE
      jsvSetFirstChild(var, 0); // firstchild could have had string data in
#endif // CLEAR_MEMORY_ON_FREE
    }

  }
  /* NO ELSE HERE - because jsvIsNewChild stuff can be for Names, which
    can be ints or strings */

  if (jsvHasChildren(var)) {
    JsVarRef childref = jsvGetFirstChild(var);
#ifdef CLEAR_MEMORY_ON_FREE
    jsvSetFirstChild(var, 0);
    jsvSetLastChild(var, 0);
#endif // CLEAR_MEMORY_ON_FREE
    while (childref) {
      JsVar *child = jsvLock(childref);
      assert(jsvIsName(child));
      childref = jsvGetNextSibling(child);
      jsvSetPrevSibling(child, 0);
      jsvSetNextSibling(child, 0);
      jsvUnRef(child);
      jsvUnLock(child);
    }
  } else {
#ifdef CLEAR_MEMORY_ON_FREE
#if JSVARREF_SIZE==1
    assert(jsvIsFloat(var) || !jsvGetFirstChild(var));
    assert(jsvIsFloat(var) || !jsvGetLastChild(var));
#else
    assert(!jsvGetFirstChild(var)); // strings use firstchild now as well
    assert(!jsvGetLastChild(var));
#endif
#endif // CLEAR_MEMORY_ON_FREE
    if (jsvIsName(var)) {
      assert(jsvGetNextSibling(var)==jsvGetPrevSibling(var)); // the case for jsvIsNewChild
      if (jsvGetNextSibling(var)) {
        jsvUnRefRef(jsvGetNextSibling(var));
        jsvUnRefRef(jsvGetPrevSibling(var));
      }
    }
  }

  jsvFreePtrInternal(var);
}
