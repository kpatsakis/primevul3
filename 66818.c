static size_t _jsvCountJsVarsUsedRecursive(JsVar *v, bool resetRecursionFlag) {
  if (!v) return 0;
  if (resetRecursionFlag) {
    if (!(v->flags & JSV_IS_RECURSING))
      return 0;
    v->flags &= ~JSV_IS_RECURSING;
  } else {
    if (v->flags & JSV_IS_RECURSING)
      return 0;
    v->flags |= JSV_IS_RECURSING;
  }

  size_t count = 1;
  if (jsvHasSingleChild(v) || jsvHasChildren(v)) {
    JsVarRef childref = jsvGetFirstChild(v);
    while (childref) {
      JsVar *child = jsvLock(childref);
      count += _jsvCountJsVarsUsedRecursive(child, resetRecursionFlag);
      if (jsvHasChildren(v)) childref = jsvGetNextSibling(child);
      else childref = 0;
      jsvUnLock(child);
    }
  } else if (jsvIsFlatString(v))
    count += jsvGetFlatStringBlocks(v);
  if (jsvHasCharacterData(v)) {
    JsVarRef childref = jsvGetLastChild(v);
    while (childref) {
      JsVar *child = jsvLock(childref);
      count++;
      childref = jsvGetLastChild(child);
      jsvUnLock(child);
    }
  }
  if (jsvIsName(v) && !jsvIsNameWithValue(v) && jsvGetFirstChild(v)) {
    JsVar *child = jsvLock(jsvGetFirstChild(v));
    count += _jsvCountJsVarsUsedRecursive(child, resetRecursionFlag);
    jsvUnLock(child);
  }
  return count;
}
