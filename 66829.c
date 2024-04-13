void jsvArrayInsertBefore(JsVar *arr, JsVar *beforeIndex, JsVar *element) {
  if (beforeIndex) {
    JsVar *idxVar = jsvMakeIntoVariableName(jsvNewFromInteger(0), element);
    if (!idxVar) return; // out of memory

    JsVarRef idxRef = jsvGetRef(jsvRef(idxVar));
    JsVarRef prev = jsvGetPrevSibling(beforeIndex);
    if (prev) {
      JsVar *prevVar = jsvRef(jsvLock(prev));
      jsvSetInteger(idxVar, jsvGetInteger(prevVar)+1); // update index number
      jsvSetNextSibling(prevVar, idxRef);
      jsvUnLock(prevVar);
      jsvSetPrevSibling(idxVar, prev);
    } else {
      jsvSetPrevSibling(idxVar, 0);
      jsvSetFirstChild(arr, idxRef);
    }
    jsvSetPrevSibling(beforeIndex, idxRef);
    jsvSetNextSibling(idxVar, jsvGetRef(jsvRef(beforeIndex)));
    jsvUnLock(idxVar);
  } else
    jsvArrayPush(arr, element);
}
