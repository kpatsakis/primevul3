JsVar *jsvArrayPop(JsVar *arr) {
  assert(jsvIsArray(arr));
  JsVar *child = 0;
  JsVarInt length = jsvGetArrayLength(arr);
  if (length > 0) {
    length--;

    if (jsvGetLastChild(arr)) {
      JsVarRef ref = jsvGetLastChild(arr);
      child = jsvLock(ref);
      while (child && !jsvIsInt(child)) {
        ref = jsvGetPrevSibling(child);
        jsvUnLock(child);
        if (ref) {
          child = jsvLock(ref);
        } else {
          child = 0;
        }
      }
      if (child) {
        if (jsvGetInteger(child) == length) {
          jsvRemoveChild(arr, child);
        } else {
          jsvUnLock(child);
          child = 0;
        }
      }
    }
    jsvSetArrayLength(arr, length, false);
  }

  return child;
}
