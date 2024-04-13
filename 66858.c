JsVar *jsvGetArrayIndex(const JsVar *arr, JsVarInt index) {
  JsVarRef childref = jsvGetLastChild(arr);
  JsVarInt lastArrayIndex = 0;
  while (childref) {
    JsVar *child = jsvLock(childref);
    if (jsvIsInt(child)) {
      lastArrayIndex = child->varData.integer;
      if (lastArrayIndex == index) {
        return child;
      }
      jsvUnLock(child);
      break;
    }
    childref = jsvGetPrevSibling(child);
    jsvUnLock(child);
  }
  if (index > lastArrayIndex)
    return 0;
  if (index > lastArrayIndex/2) {
    while (childref) {
      JsVar *child = jsvLock(childref);

      assert(jsvIsInt(child));
      if (child->varData.integer == index) {
        return child;
      }
      childref = jsvGetPrevSibling(child);
      jsvUnLock(child);
    }
  } else {
    childref = jsvGetFirstChild(arr);
    while (childref) {
      JsVar *child = jsvLock(childref);

      assert(jsvIsInt(child));
      if (child->varData.integer == index) {
        return child;
      }
      childref = jsvGetNextSibling(child);
      jsvUnLock(child);
    }
  }
  return 0; // undefined
}
